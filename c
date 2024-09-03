_G.FastAttack = true
_G.FastAttackDelay = 0.1
_G.NotDeley = true
function GetBladeHit()
    local CombatFrameworkLib = debug.getupvalues(require(game:GetService("Players").LocalPlayer.PlayerScripts.CombatFramework))
    local CmrFwLib = CombatFrameworkLib[2]
    local p13 = CmrFwLib.activeController
    local weapon = p13.blades[1]
    if not weapon then 
        return weapon
    end
    while weapon.Parent ~= game.Players.LocalPlayer.Character do
        weapon = weapon.Parent 
    end
    return weapon
end
function AttackHit()
    local CombatFrameworkLib = debug.getupvalues(require(game:GetService("Players").LocalPlayer.PlayerScripts.CombatFramework))
    local CmrFwLib = CombatFrameworkLib[2]
    local plr = game.Players.LocalPlayer
    for i = 1, 1 do
        local bladehit = require(game.ReplicatedStorage.CombatFramework.RigLib).getBladeHits(plr.Character,{plr.Character.HumanoidRootPart},60)
        local cac = {}
        local hash = {}
        for k, v in pairs(bladehit) do
            if v.Parent:FindFirstChild("HumanoidRootPart") and not hash[v.Parent] then
                table.insert(cac, v.Parent.HumanoidRootPart)
                hash[v.Parent] = true
            end
        end
        bladehit = cac
        if #bladehit > 0 then
            pcall(function()
                CmrFwLib.activeController.timeToNextAttack = 1
                CmrFwLib.activeController.attacking = false
                CmrFwLib.activeController.blocking = false
                CmrFwLib.activeController.timeToNextBlock = 0
                CmrFwLib.activeController.increment = 3
                CmrFwLib.activeController.hitboxMagnitude = 60
                CmrFwLib.activeController.focusStart = 0
                game:GetService("ReplicatedStorage").RigControllerEvent:FireServer("weaponChange",tostring(GetBladeHit()))
                game:GetService("ReplicatedStorage").RigControllerEvent:FireServer("hit", bladehit, i, "")
            end)
        end
    end
end
spawn(function()
    while wait(.1) do
        if _G.FastAttack then
            pcall(function()
                repeat task.wait(_G.FastAttackDelay)
                    AttackHit()
                until not _G.FastAttack
            end)
        end
    end
end)
local Client = game.Players.LocalPlayer
local STOP = require(Client.PlayerScripts.CombatFramework.Particle)
local STOPRL = require(game:GetService("ReplicatedStorage").CombatFramework.RigLib)
task.spawn(function()
    pcall(function()
        if not shared.orl then
            shared.orl = STOPRL.wrapAttackAnimationAsync
        end
        if not shared.cpc then
            shared.cpc = STOP.play 
        end
        spawn(function()
            require(game.ReplicatedStorage.Util.CameraShaker):Stop()
            require(game.ReplicatedStorage.Util.CameraShaker):Stop()
            game:GetService("RunService").Stepped:Connect(function()
                STOPRL.wrapAttackAnimationAsync = function(a,b,c,d,func)
                    local Hits = STOPRL.getBladeHits(b,c,d)
                    if Hits then
                        if  _G.FastAttack then
                            STOP.play = function() end
                            a:Play(20.1,15.1,10.1)
                            func(Hits)
                            STOP.play = shared.cpc
                            wait(a.length * 1000000000.5)
                            a:Stop()
                        else
                            func(Hits)
                            STOP.play = shared.cpc
                            wait(a.length * 10000000000.5)
                            a:Stop()
                        end
                    end
                end
            end)
        end)
    end)
end)

spawn(function()
    game:GetService("RunService").RenderStepped:Connect(function()
        if _G.NotDeley then
             pcall(function()
                AttackHit()
            end)
        end
    end)
end)
