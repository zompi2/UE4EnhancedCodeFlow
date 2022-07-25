# Enhanced Code Flow for Unreal Engine 4

This code plugin provides functions that drastically improve the quality of life during the implementation of game flow in C++.  
It works very well with gameplay programming, UI programming with a lot of transitions or in any other situation.

# Table of content

- [Usage](#usage)
- [Extra Settings](#extra-settings)
- [Instanced Actions](#instanced-actions)
- [Pausing and Resuming Actions](#pausing-and-resuming-actions)
- [Stopping Actions](#stopping-actions)
- [Extending Plugin](#extending-plugin)
- [Special Thanks](#special-thanks)

# Contact

If you have any question or suggestion regardles this plugin simply add an **Issue** to the github project or write an e-mail to me: **zompi2@gmail.com** I will try my best to answer it quickly :)

# Changelog

The Changelog has been put into this file: **[Changelog.txt](Changelog.txt)**

**IMPORTANT**
Version `2.0.0` will probably break Blueprint nodes from previous versions. Update with caution!  
Version `1.6.1` can be found on a separate branch here: [TODO]

# Example Project

The example project wich uses this plugin can be found in **[this repository](https://github.com/zompi2/UE4EnhancedCodeFlowExample)**. Example project is compatible with the newest version of the plugin only.

![ecfscreen](https://user-images.githubusercontent.com/7863125/143097137-fe37ee9d-ccaf-440a-900f-86568ef77883.png)

# Usage

- [Delay](#delay)
- [Add Ticker](#add-ticker)
- [Wait And Execute](#wait-and-execute)
- [While True Execute](#while-true-execute)
- [Add Timeline](#add-timeline)
- [Add Custom Timeline](#add-custom-timeline)
- [Time Lock](#time-lock)
- [Do Once](#do-once)
- [Do N Times](#do-n-times)

Run the following functions to use enhanced code flow!

> Note that every function must receive a pointer to an owner that runs this function in it's first argument.  
> The owner must be able to return a World via **GetWorld()** function.

#### Delay

Execute specified action after some time. This can be useful in many various situations. Everytime when I was using a Delay node
in blueprints I wish there was an equivalent of it in c++.

``` cpp
FFlow::Delay(this, 2.f, [this]()
{
  // Code to execute after 2 seconds.
});
```

An ECF-Delay BP node has few advantages over the built in Unreal's Delay node.  
You can plan to execute delayed code without delaying the whole Blueprint, you can cancel the delayed code's execution or make the dilation game pause and time dilation independent. 

![delay_bp](https://user-images.githubusercontent.com/7863125/180840927-38ebd2b4-9fe6-45fa-867d-03f76dd76361.png)

[Back to actions list](#usage)  
[Back to top](#table-of-content)

#### Add Ticker

Creates a ticker. It can tick specified amount of time or until it won't be stopped or when owning object won't be destroyed.  
Useful for actors and components that you don't want to be tickeable, but needs one tick to do something.

**Run ticker for 10 seconds**

``` cpp
FFlow::AddTicker(this, 10.f, [this](float DeltaTime)
{
  // Code to execute every tick
});
```

**Run ticker for 10 seconds and run a callback when it finishes**

``` cpp
FFlow::AddTicker(this, 10.f, [this](float DeltaTime)
{
  // Code to execute every tick
}, [this]()
{
  // Code to execute when ticker finishes ticking
});
```

**Run ticker for infinite time and stop it when you want to**

``` cpp
FFlow::AddTicker(this, [this](float DeltaTime, FECFHandle TickerHandle)
{
  // Code to execute in every tick.

  // Use this to stop the ticker
  FFlow::StopAction(this, TickerHandle);
});
```

**Run ticker for infinite time and something else stops it**

``` cpp
FECFHandle TickerHandle = FFlow::AddTicker(this, [this](float DeltaTime)
{
  // Code to execute in every tick.
});

// Use this to stop the ticker
FFlow::StopAction(this, TickerHandle);
```

> Note 1: Tickers and every other plugin actions are impacted by global time dilation.  
> Note 2: You can check if the ticker (or any other action) is running using **FFlow::IsActionRunning(TickerHandle)**  
> Note 3: You can also run ticker infinitely by setting Ticking Time to -1

![ticker_bp](https://user-images.githubusercontent.com/7863125/180840930-f93cfb16-c5d9-4270-802b-e428fe92c17d.png)

[Back to actions list](#usage)  
[Back to top](#table-of-content)

#### Wait and execute

Waits until specific conditions are met and then executes code.  
The conditions are defined in a form of predicate.  
Perfect solution if code needs a reference to an object, which spawn moment is not clearly defined, or if you can execute a specific code only when the game reaches a specific state. 

``` cpp
FFlow::WaitAndExecute(this, [this]()
{
  // Write your own predicate. 
  // Return true when you want to execute the code below.
  return bIsReadyToUse;
},
[this]()
{
  // Implement code to execute when conditions are met.
});
```

BP version of this function uses a `Predicate` function which controls when the `On Execution` pin will execute.

![waitandex_bp](https://user-images.githubusercontent.com/7863125/180840932-b0e99740-4541-4e86-aeb1-938d1c732c00.png)

[Back to actions list](#usage)  
[Back to top](#table-of-content)

#### While true execute

While the specified conditions are true tick the given code.  
This one is useful when you want to write a loop that executes one run every tick until it finishes it's job.  

``` cpp
FFlow::WhileTrueExecute(this, [this]()
{
  // Write your own predicate. 
  // Return true when you want this action to continue.
  return bIsRunning;
},
[this](float DeltaTime)
{
  // Implement code to tick when predicate returns true.
});
```

BP version of this function uses a `Predicate` function which controls when the `On Execution` pin with `Delta Time` will execute.

![whiletrue_bp](https://user-images.githubusercontent.com/7863125/180841417-7674bca9-54ae-4c90-b986-4510b547d3c6.png)

[Back to actions list](#usage)  
[Back to top](#table-of-content)

#### Add timeline

Easily launch the timeline and update your game based on them. Great solution for any kind of blends and transitions.
The function requires the following parameters:  
* StartValue - a value with which the timeline will begin;
* StopValue - a value with which the timeline will end. StopValue can be lesser than StartValue;
* Time - how long the timeline will work;
* TickFunc - a function that will tick with the timeline. It has the following arguments:
  * Value - a current value on this timeline;
  * Time - a time that passed on this timeline;
* CallbackFunc - a function that will run when the timeline comes to an end. Has the same arguments as TickFunc. This function is *optional*;
* BlendFunc - a function that describes a shape of the timeline:
  * Linear *(default)*
  * Cubic
  * EaseIn
  * EaseOut
  * EaseInOut
* BlendExp - an exponent defining a shape of EaseIn, EaseOut and EaseInOut function shapes. *(default value: 1.f)*;

``` cpp
FFlow::AddTimeline(this, 0.f, 1.f, 2.f, [this](float Value, float Time)
{
  // Code to run every time the timeline tick
}, 
[this](float Value, float Time)
{
  // Code to run when timeline stops
}, 
EECFBlendFunc::ECFBlend_Linear, 2.f);
```

![timeline_bp](https://user-images.githubusercontent.com/7863125/180841988-1c1ec2eb-0a44-41fe-ad31-b438ebf268c7.png)

[Back to actions list](#usage)  
[Back to top](#table-of-content)

#### Add custom timeline

Creates a discrete timeline which shape is based on a **UCurveFloat**. Works like the previously described timeline, but an asset with a curve must be given.

``` cpp
FFlow::AddCustomTimeline(this, Curve, [this](float Value, float Time)
{
  // Code to run every time the timeline tick
}, 
[this](float Value, float Time)
{
  // Code to run when timeline stops
});
```

![custim_bp](https://user-images.githubusercontent.com/7863125/180842199-5d9bba5a-8255-4dbf-8e27-4a4530cc2c53.png)

[Back to actions list](#usage)  
[Back to top](#table-of-content)

#### Time Lock

**(Instanced)**

Blocks execution of the block of code until the given time has passed.

``` cpp
static FECFInstanceId InstanceId = FECFInstanceId::NewId();
FFlow::TimeLock(this, 2.f, [this]()
{
  // This code will run now, and won't be able to execute for 2 seconds.
}, InstanceId);
```

BP version of this function requires `InstanceId` too. The best way to obtain it is to validate `InstanceId` handler. It will return a new `InstanceId` only if the one in the handler is not valid.

![timelock_bp](https://user-images.githubusercontent.com/7863125/180842599-84917c05-4036-4924-b15f-c5f7d5e95480.png)

[Back to actions list](#usage)  
[Back to top](#table-of-content)

#### Do Once

**(Instanced)**

Allow to execute the given block of code only once.

``` cpp
static FECFInstanceId InstanceId = FECFInstanceId::NewId();
FFlow::DoOnce(this, [this]()
{
  // This code can be run only once.
}, InstanceId);
```
> This function doesn't have a BP version, because Unreal has one already.

[Back to actions list](#usage)  
[Back to top](#table-of-content)

#### Do N Times

**(Instanced)**

Allow to execute the given block of code only given amount of times.

``` cpp
static FECFInstanceId InstanceId = FECFInstanceId::NewId();
FFlow::DoNTimes(this, 5, [this](int32 Counter)
{
  // This code can be run only 5 times.
}, InstanceId);
```

> This function doesn't have a BP version, because Unreal has one already.

[Back to actions list](#usage)  
[Back to top](#table-of-content)

# Extra settings

You can define extra settings at the end of each action launch. Currently the following actions are available:
* Time Intervals - defines the length of one tick.
* First Delay - defines when the first tick should be performed.
* Ignore Game Pause - it will ignore the game pause.
* Ignore Global Time Dilation - it will ignore global time dilation when ticking.
* Start Paused - the action will start in paused state and must be resumed manually.

``` cpp
FFlow::AddTicker(this, 10.f, [this](float DeltaTime)
{
  // Code to execute every 1 second for 10 seconds.
}, nullptr, FECFActionSettings(1.f, 0.f, false, false, false));
```

``` cpp
FFlow::AddTicker(this, 10.f, [this](float DeltaTime)
{
  // Code to execute every tick for 10 seconds 
  // while ignoring global time dilation.
}, nullptr, FECFActionSettings(0.f, 0.f, true, false, false));
```

``` cpp
FFlow::AddTicker(this, 10.f, [this](float DeltaTime)
{
  // Code to execute every 1 seconds for 10 seconds, 
  // after 5 seconds have passed, while ignoring 
  // global time dilation and pause.
}, nullptr, FECFActionSettings(1.f, 5.f, true, true, false));
```

To make defining these settings easier there are few macros that creates a settings structure with just one option:

* `ECF_TICKINTERVAL(5.f)` - settings which sets tick interval to 5 second
* `ECF_DELAYFIRST(1.f)` - settings which makes this action to run after 1 second delay
* `ECF_IGNOREPAUSE` - settings which makes this action ignore game pause
* `ECF_IGNORETIMEDILATION` - settings which makes this action ignore global time dilation
* `ECF_IGNOREPAUSEDILATION` - settings which makes this action ignore pause and global time dilation
* `ECF_STARTPAUSED` - settings which makes this action started in paused state

``` cpp
FFlow::Delay(this, 2.f, [this]()
{
  // Run this code after 2 seconds, while ignoring game pause.
}, ECF_IGNOREPAUSE);
```

![sett](https://user-images.githubusercontent.com/7863125/180844848-3dc7106a-02af-421a-ab9e-4190ab3a4477.png)

[Back to top](#table-of-content)

# Instanced Actions

Some actions can be Instanced. Instanced action is an action that has valid **`FECFInstanceId`**. Such action can be executed only once. 

As long as the action with given valid `FECFInstanceId` is running, no other action with the same `FECFInstanceId` can be executed.

#### Obtaining InstanceId

To get next valid InstanceId use the `NewId()` function

``` cpp
FECFInstanceId::NewId();
```

There is additional BP node which will validate an `InstanceId` if it is not valid. 

![instid](https://user-images.githubusercontent.com/7863125/180844002-8741634d-7c7e-4407-9736-f73417b366c7.png)

[Back to top](#table-of-content)

# Pausing and Resuming
## Actions

Every running action can be paused and resumed.

``` cpp
FFlow::PauseAction(GetWorld(), Handle); // Pause Action
FFlow::ResumeAction(GetWorld(), Handle); // Resume Action

// Checks if the Action is Running and if it is Paused or not
bool bIsPaused;
bool bIsRunning = FFlow::IsActionPaused(WorldContextObject, Handle, bIsPaused);
```

![pauseas](https://user-images.githubusercontent.com/7863125/180850860-c6548e29-9678-4e22-b70d-f3c22cd74dc7.png)

## Subsystem
Whole Enhanced Code Subsystem can be paused and resumed as well.

``` cpp
FFlow::SetPause(GetWorld(), true); // Pauses the whole ECF Subsystem
FFlow::SetPause(GetWorld(), false); // Unpauses the whole ECF Subsystem
FFlow::GetPause(GetWorld()); // Check if the whole ECF Subsystem is paused or not
```

![pausesub](https://user-images.githubusercontent.com/7863125/180851156-863f90f2-07f1-4082-9c46-dd22944d4686.png)

[Back to top](#table-of-content)

# Stopping actions

Every function described earlier can be checked if it's running and it can be stopped.

``` cpp
FFlow::IsActionRunning(GetWorld(), Handle); // <- is this action running?
FFlow::StopAction(GetWorld(), Handle); // <- stops this action!
```

You can also stop all of the actions from a specific owner or from everywhere.  
Stopped actions can launch their **completion** callbacks or not, depending on the given argument:

``` cpp
FFlow::StopAllActions(GetWorld()); // <- stops all of the actions
FFlow::StopAllActions(GetWorld(), true); // <- stops all of the actions and launch their callbacks
FFlow::StopAllActions(GetWorld(), false, Owner); // <- stops all of the actions started from this specific owner
```

![stopping](https://user-images.githubusercontent.com/7863125/180849533-03cb9d37-977f-4c9e-8961-aebd60f8ee25.png)

You can also stop a specific Instanced action with the **`FECFInstanceId`**:

``` cpp
FFlow::StopInstancedAction(GetWorld(), InstanceId); // <- stops all actions with this InstanceId
FFlow::StopInstancedAction(GetWorld(), InstanceId, true); // <- stops all actions with this InstanceId and launch their callbacks
```

![stopinst](https://user-images.githubusercontent.com/7863125/180849970-246f8d85-33c0-406c-af23-da4cd9244019.png)

You can also stop all of the **specific** actions. In this case you can also optionally specifiy an owner of this actions, or simply stop all of them.
You can also specify if stopped actions should launch their **completion** callbacks or not.

``` cpp
FFlow::RemoveAllDelays(GetWorld());
FFlow::RemoveAllTickers(GetWorld());
FFlow::RemoveAllWaitAndExecutes(GetWorld());
FFlow::RemoveAllWhileTrueExecutes(GetWorld());
FFlow::RemoveAllTimelines(GetWorld());
FFlow::RemoveAllCustomTimelines(GetWorld());
FFlow::RemoveAllTimeLocks(GetWorld());
```

![removeall](https://user-images.githubusercontent.com/7863125/180850420-b5082914-87ca-443c-9be6-afb8055af9d0.png)

[Back to top](#table-of-content)

# Extending plugin

You have a source code of this plugin you can easily extend it's functionalities!

> Check how other actions are made to easier understand how to extend the plugin.

1. Create a class that inherits from `UECFActionBase`
2. Implement `Setup` function, which accepts all parameters you want to pass to this action. 
   `Setup` function must return true if the given parameters are valid.  
```cpp
bool Setup(int32 Param1, int32 Param2, TUniqueFunction<void()>&& Callback)
{
  CallbackFunc = MoveTemp(Callback);
  if (CallbackFunc) return true;
  return false;
}
```
> Any callback must be passed as an r-value reference and be moved to the action's variable.

3. Override `Init` and `Tick` functions if needed.
4. If you want this action to be stopped while ticking - use `MarkAsFinished()` function.
5. If you want to launch a callback when this action is stopped by `StopAction` method with `bComplete` set to true - override `Complete()` function.
6. If this is an instanced action you can optionally override `RetriggeredInstancedAction()` function to add any logic that should be executed when the instanced action is called while already existing.
7. You can optionally run `SetMaxActionTime` in action's `Init` function to determine the maximum time in seconds this action should run. 
>IMMPORTANT! SetMaxActionTime is only to help ticker run ticks with proper delta times.  
>It will not stop the action itself!
8. In the `FEnhancedCodeFlow` class implement static function that launches the action using `AddAction` function.
   The function must receive a pointer to the launching `UObject`, `FECFActionSettings`, `FECFInstanceId` (use invalid one if the action shouldn't be instanced) and every other argument that is used in the action's `Setup` function in the same order.
   It must return `FECFHandle`.
```cpp
FECFHandle FEnhancedCodeFlow::NewAction(UObject* InOwner, int32 Param1, int32 Param2, TUniqueFunction<void()>&& Call, const FECFActionSettings& Settings = {})
{
  if (UECFSubsystem* ECF = UECFSubsystem::Get(InOwner))
    return ECF->AddAction<UECFNewAction>(InOwner, Settings, FECFInstanceId(), Param1, Param2, MoveTemp(Call));
  else
    return FECFHandle();
}
```

It is done! Now you can run your own action:

```cpp
FFlow::NewAction(this, 1, 2, [this]()
{
  // Callback code.
}, ECF_IGNOREPAUSE);
```
[Back to top](#table-of-content)

# Special thanks

I want to send special thanks to Monika, because she always supports me and believes in me, to Pawel, for allowing me to test this plugin on his project and to everyone that contributed to this project.  
Also, I want to thank You for using this plugin! It is very important for me that my work is useful for someone!  
Happy coding!

[Back to top](#table-of-content)
