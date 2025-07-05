This repository marked a long journey of learning for me in Unreal. I thank everyone for their support to this repository, even though the final version is not 100% bug-free XD.

New ALS repository: https://github.com/Cesio137/ALS_Replicated/tree/master

# ALSReplicated UE 4.26 | 4.27 | 5.1 | 5.2

This is a community-based effort to fully and effectively replicate Advanced Locomotion System v4 which is permanently free on the Epic Marketplace. 

Discussion regarding the replication effort of ALS should take place on the official discord for Advanced Locomotion System here: https://discord.gg/wYYMHFu

<p align="center">
  <a href="https://discord.gg/wYYMHFu"><img src="https://i.imgur.com/LP9bZQj.png"></a>
</p>

This repository will operate on a series of pull requests. You are free to download modify and pull request your modifications in. If it meets with the criteria of effectively replicating the project, it will be reviewed and merged in.

## Sponsor me

<p align="center">
  <a href="https://www.paypal.com/donate?hosted_button_id=L48BPZ4VVCN6Q"><img src="https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif"></a>
</p>
<p align="center">
  <a href="https://nubank.com.br/pagar/1bcou4/5D6eezlHdm"><img src="https://logodownload.org/wp-content/uploads/2020/02/pix-bc-logo.png" width="128"></a>
</p>

### Bug Reporting Template:

```
**Detailed description of issue**
Write a detailed explanation of the issue here.

**Steps To Reproduce:**
1: Detailed Steps to reproduce the issue 
2: Clear steps
3: Etc

**Expected Results:**
A description of what should happen.

**Actual Results:**
A description of what actually happens.
```

# Setting Up Your Project

- Install VS 2019/2022 and Engine sources.

- Clone the repository or download the latest release.

- Move `ALSReplicated` folder into your project's `Plugins` folder

- Add the lines below into your project's `DefaultInput.ini`, below `[/Script/Engine.InputSettings]` tag:
  
  ```
  +ActionMappings=(ActionName="JumpAction",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=SpaceBar)
  +ActionMappings=(ActionName="JumpAction",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=Gamepad_FaceButton_Bottom)
  +ActionMappings=(ActionName="StanceAction",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=LeftAlt)
  +ActionMappings=(ActionName="SprintAction",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=LeftShift)
  +ActionMappings=(ActionName="StanceAction",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=Gamepad_FaceButton_Right)
  +ActionMappings=(ActionName="SprintAction",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=Gamepad_LeftThumbstick)
  +ActionMappings=(ActionName="WalkAction",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=LeftControl)
  +ActionMappings=(ActionName="WalkAction",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=Gamepad_RightShoulder)
  +ActionMappings=(ActionName="AimAction",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=RightMouseButton)
  +ActionMappings=(ActionName="AimAction",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=Gamepad_LeftTrigger)
  +ActionMappings=(ActionName="SelectRotationMode_1",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=One)
  +ActionMappings=(ActionName="SelectRotationMode_2",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=Two)
  +ActionMappings=(ActionName="SelectRotationMode_1",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=Gamepad_DPad_Left)
  +ActionMappings=(ActionName="SelectRotationMode_2",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=Gamepad_DPad_Right)
  +ActionMappings=(ActionName="CameraAction",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=C)
  +ActionMappings=(ActionName="CameraAction",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=Gamepad_RightThumbstick)
  +ActionMappings=(ActionName="RagdollAction",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=X)
  +ActionMappings=(ActionName="RagdollAction",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=Gamepad_Special_Left)
  +ActionMappings=(ActionName="CycleOverlayUp",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=MouseScrollUp)
  +ActionMappings=(ActionName="CycleOverlayDown",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=MouseScrollDown)
  +ActionMappings=(ActionName="CycleOverlayUp",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=Gamepad_DPad_Up)
  +ActionMappings=(ActionName="CycleOverlayDown",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=Gamepad_DPad_Down)
  +ActionMappings=(ActionName="OpenOverlayMenu",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=Q)
  +ActionMappings=(ActionName="OpenOverlayMenu",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=Gamepad_LeftShoulder)
  +ActionMappings=(ActionName="UseAction",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=E)
  +ActionMappings=(ActionName="LockOnAction",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=MiddleMouseButton)
  +AxisMappings=(AxisName="MoveForward/Backwards",Scale=1.000000,Key=W)
  +AxisMappings=(AxisName="MoveRight/Left",Scale=1.000000,Key=D)
  +AxisMappings=(AxisName="LookUp/Down",Scale=-1.000000,Key=MouseY)
  +AxisMappings=(AxisName="LookLeft/Right",Scale=1.000000,Key=MouseX)
  +AxisMappings=(AxisName="MoveForward/Backwards",Scale=-1.000000,Key=S)
  +AxisMappings=(AxisName="MoveRight/Left",Scale=-1.000000,Key=A)
  +AxisMappings=(AxisName="MoveForward/Backwards",Scale=1.000000,Key=Gamepad_LeftY)
  +AxisMappings=(AxisName="MoveRight/Left",Scale=1.000000,Key=Gamepad_LeftX)
  +AxisMappings=(AxisName="LookUp/Down",Scale=1.000000,Key=Gamepad_RightY)
  +AxisMappings=(AxisName="LookLeft/Right",Scale=1.000000,Key=Gamepad_RightX)
  ```

- Add the lines below into your `DefaultEngine.ini`, below `[/Script/Engine.CollisionProfile]` tag (Create the tag if it doesn't exist):
  
  ```
  +Profiles=(Name="ALS_Character",CollisionEnabled=QueryAndPhysics,bCanModify=True,ObjectTypeName="Pawn",CustomResponses=((Channel="Visibility",Response=ECR_Ignore),(Channel="Camera",Response=ECR_Ignore),(Channel="Climbable",Response=ECR_Ignore)),HelpMessage="Custom collision settings for the capsule in the ALS_BaseCharacter.")
  +DefaultChannelResponses=(Channel=ECC_GameTraceChannel2,DefaultResponse=ECR_Block,bTraceType=True,bStaticObject=False,Name="Climbable")
  ```

## Blueprint Events

`UCombatComponent` exposes the `OnAttackStateChanged` event which is triggered whenever an attack begins or ends. Implement this in your blueprint to play effects or update the UI when `bAttacking` is `true` or `false`.


## Automation Tests

A set of automation tests validates component replication. Open **Session Frontend** in the Unreal Editor and run the tests under `ALSReplicated.ComponentReplication`, `ALSReplicated.StaminaReplication`, `ALSReplicated.LockOn.TargetReplication` and `ALSReplicated.EnvironmentInteractionReplication`. Alternatively, run them from the command line:

```
UE4Editor.exe <YourProject>.uproject -run=Automation -test=ALSReplicated.* -unattended
```

## Interactive Actor Setup

Interactive objects that you intend to push, pull or otherwise move should replicate their movement. Ensure the actor has **bReplicateMovement** enabled or that you manually replicate its transform. For the best network prediction during push and pull, use a `UPrimitiveComponent` with physics simulation and call `SetPhysicsLinearVelocity` rather than directly updating the actor transform.

