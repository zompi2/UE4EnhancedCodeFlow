# Enhanced Code Flow for Unreal Engine 4

This code plugin provides functions that drastically improve the quality of life during the implementation of game flow in C++.  
It works very well with gameplay programming, UI programming with a lot of transitions or in any other situation.

# Table of content

- [Usage](#usage)
- [Extra Settings](#extra-settings)
- [Instanced Actions](#instanced-actions)
- [Pausing and Resuming](#pausing-and-resuming)
- [Stopping Actions](#stopping-actions)
- [Measuring Performance](#measuring-performance)
- [Extending Plugin](#extending-plugin)
- [Extra Links](#extra-links)
- [Special Thanks](#special-thanks)

# Contact

If you have any question or suggestion regardles this plugin simply add an **Issue** to the github project or write an e-mail to me: **zompi2@gmail.com** I will try my best to answer it quickly :)

# Changelog

The Changelog has been put into this file: **[Changelog.txt](Changelog.txt)**

## **IMPORTANT**  
Version `3.0.0` will probably break code and Blueprint nodes from previous version. Update with caution!  
Version `2.1.2` can be found on a separate branch here: **Legacy-2.1**  

Version `2.0.0` will probably break Blueprint nodes from previous versions. Update with caution!  
Version `1.6.1` can be found on a separate branch here: **[Legacy-1.6](https://github.com/zompi2/UE4EnhancedCodeFlow/tree/Legacy-1.6)**

# Example Project

The example project wich uses this plugin can be found in **[this repository](https://github.com/zompi2/UE4EnhancedCodeFlowExample)**. Example project is compatible with the newest version of the plugin only.

![Main](https://user-images.githubusercontent.com/7863125/218276144-932eba74-913f-4376-afac-26aa1ef13e30.png)

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
- [Do No More Than X Time](#do-no-more-than-x-time)

Run the following functions to use enhanced code flow!

> Note that every function must receive a pointer to an owner that runs this function in it's first argument.  
> The owner must be able to return a World via **GetWorld()** function.

#### Delay

Execute specified action after some time. This can be useful in many various situations. Everytime when I was using a Delay node
in blueprints I wish there was an equivalent of it in c++. The `bStopped` tells if this action has been stopped by a Stop function.

``` cpp
FFlow::Delay(this, 2.f, [this](bool bStopped)
{
  // Code to execute after 2 seconds.
});
```

An ECF-Delay BP node has few advantages over the built in Unreal's Delay node.  
You can plan to execute delayed code without delaying the whole Blueprint, you can cancel the delayed code's execution or make the dilation game pause and time dilation independent. 

![Delay](https://user-images.githubusercontent.com/7863125/218276143-db9554f2-abb3-40a1-ad83-ad1132812bb7.png)

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
}, [this](bool bStopped)
{
  // Code to execute when ticker finishes ticking.
  // The bStopped tells if this action has been stopped by a Stop function.
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

![Ticker](https://user-images.githubusercontent.com/7863125/218276146-fe27c97e-911d-4af1-980e-54556efc4f08.png)

[Back to actions list](#usage)  
[Back to top](#table-of-content)

#### Wait and execute

Waits until specific conditions are met and then executes code.  
The conditions are defined in a form of a predicate.  
You can specify a timeout, which will stop this action after the given time. Setting the timeout value to less or equal 0 will cause this function to run infinitely untill the predicate returns true or when it is explicitly stopped.  
The `bStopped` tells if this action has been stopped by a Stop function.  
Perfect solution if code needs a reference to an object, which spawn moment is not clearly defined, or if you can execute a specific code only when the game reaches a specific state. 


``` cpp
FFlow::WaitAndExecute(this, [this]()
{
  // Write your own predicate. 
  // Return true when you want to execute the code below.
  return bIsReadyToUse;
},
[this](bool bTimedOut, bool bStopped)
{
  // Implement code to execute when conditions are met or when this action has ran for 5 seconds (time specified in a timeout parameter)
}, 5.f);
```

BP version of this function uses a `Predicate` function which controls when the `On Execution` pin will execute.

![WaitAndExecute](https://user-images.githubusercontent.com/7863125/218276148-2cb4feec-7343-4a63-92a4-2f0334e495c0.png)

[Back to actions list](#usage)  
[Back to top](#table-of-content)

#### While true execute

While the specified conditions are true tick the given code.  
This one is useful when you want to write a loop that executes one run every tick until it finishes it's job.  
You can specify a timeout, which will stop this action after the given time. Setting the timeout value to less or equal 0 will cause this function to run infinitely untill the predicate returns false or when it is explicitly stopped.  
You can optionally defined what happens when the loop ends.  

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
},
[this](bool bTimedOut, bool bStopped)
{
  // Optionally implement a code that runs when this action ends, even when the condition
  // in the predicate returns false or it is timed out or it is explicitly stopped.
}, 0.f);
```

BP version of this function uses a `Predicate` function which controls when the `On Execution` pin with `Delta Time` will execute.

![WhileTrueExecute](https://user-images.githubusercontent.com/7863125/218276149-964b91e0-76da-4758-b2c2-7800a0eea2ae.png)

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

The `bStopped` tells if this action has been stopped by a Stop function.  

``` cpp
FFlow::AddTimeline(this, 0.f, 1.f, 2.f, [this](float Value, float Time)
{
  // Code to run every time the timeline tick
}, 
[this](float Value, float Time, bool bStopped)
{
  // Code to run when timeline stops
}, 
EECFBlendFunc::ECFBlend_Linear, 2.f);
```

![Timeline](https://user-images.githubusercontent.com/7863125/218276147-80928cc9-5455-4edd-bd7c-2f50ae819ca3.png)

[Back to actions list](#usage)  
[Back to top](#table-of-content)

#### Add custom timeline

Creates a discrete timeline which shape is based on a **UCurveFloat**. Works like the previously described timeline, but an asset with a curve must be given.

``` cpp
FFlow::AddCustomTimeline(this, Curve, [this](float Value, float Time)
{
  // Code to run every time the timeline tick
}, 
[this](float Value, float Time, bool bStopped)
{
  // Code to run when timeline stops
});
```

![CustomTimeline](https://user-images.githubusercontent.com/7863125/218276141-1168dd7d-24ab-43bd-901a-bedb3fb9664b.png)

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

BP version of this function requires `InstanceId` too. The BP node will validate the `InstandeId` from the handler so it just need to be passed into it.

![tlock1](https://user-images.githubusercontent.com/7863125/201354732-26bd20b3-f6b1-433e-8eef-19d0e6e4189d.png)

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

#### Do No More Than X Time

**(Instanced)**

It will execute the given block of code immediately, but the next execution will be enqueued and will be called after specified time. There is a parameter which allow to define how many next executions can be enqueued (must be at least 1). If this code will be used when the queue is full - the code will be discarded (not enqueued).

``` cpp
static FECFInstanceId InstanceId = FECFInstanceId::NewId();
FFlow::DoNoMoreThanXTime(this, [this]()
{
  // This code will run now and if called again it will run no earlier than after 5 seconds after the last execution.
  // If this will be called again before the second execution - it will be discarded.
}, 5.f, 1, InstanceId);
```

BP version of this function requires `InstanceId` too. The BP node will validate the `InstandeId` from the handler so it just need to be passed into it.

![donomor](https://user-images.githubusercontent.com/7863125/201354730-e444acc0-c327-48c1-b0f6-3d6cc5855362.png)

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
FFlow::Delay(this, 2.f, [this](bool bStopped)
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

When the **completion** callback will run after the Stop Function, the `bStopped` argument in the completion function of the action will be set to `true`.

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
FFlow::RemoveAllDoNoMoreThanXTimes(GetWorld());
```

![removeall](https://user-images.githubusercontent.com/7863125/201354733-31eed266-097a-45b6-8733-e4e17a306ed9.png)

[Back to top](#table-of-content)

# Measuring Performance

To measure plugin's performance you can use the stat command designed for it:  
`stat ecf`  
![stat](https://user-images.githubusercontent.com/7863125/218276145-f0e80f4e-70fa-4c62-80e7-2b132e2b0bf7.png)  
There are the following stats:  
* Tick - the time in `ms` the plugin needs to perform one full update.
* Actions - the amount of actions that are currently running.
* Instances - describes how many of the running actions are the instanced ones.

[Back to top](#table-of-content)

# Extending plugin

You have a source code of this plugin you can easily extend it's functionalities!

> Check how other actions are made to easier understand how to extend the plugin.

1. Create a class that inherits from `UECFActionBase`
2. Implement `Setup` function, which accepts all parameters you want to pass to this action. 
   `Setup` function must return true if the given parameters are valid.  
```cpp
bool Setup(int32 Param1, int32 Param2, TUniqueFunction<void(bool)>&& Callback)
{
  CallbackFunc = MoveTemp(Callback);
  if (CallbackFunc) return true;
  return false;
}
```
> Any callback must be passed as an r-value reference and be moved to the action's variable.

3. Override `Init` and `Tick` functions if needed.
4. If you want this action to be stopped while ticking - use `MarkAsFinished()` function.
5. If you want to launch a callback when this action is stopped by `StopAction` method with `bComplete` set to true - override `Complete(bool bStopped)` function.
6. If this is an instanced action you can optionally override `RetriggeredInstancedAction()` function to add any logic that should be executed when the instanced action is called while already existing.
7. You can optionally run `SetMaxActionTime` in action's `Init` function to determine the maximum time in seconds this action should run. 
>IMMPORTANT! SetMaxActionTime is only to help ticker run ticks with proper delta times.  
>It will not stop the action itself!
8. In the `FEnhancedCodeFlow` class implement static function that launches the action using `AddAction` function.
   The function must receive a pointer to the launching `UObject`, `FECFActionSettings`, `FECFInstanceId` (use invalid one if the action shouldn't be instanced) and every other argument that is used in the action's `Setup` function in the same order.
   It must return `FECFHandle`.
```cpp
FECFHandle FEnhancedCodeFlow::NewAction(const UObject* InOwner, int32 Param1, int32 Param2, TUniqueFunction<void(bool)>&& Callback, const FECFActionSettings& Settings = {})
{
  if (UECFSubsystem* ECF = UECFSubsystem::Get(InOwner))
    return ECF->AddAction<UECFNewAction>(InOwner, Settings, FECFInstanceId(), Param1, Param2, MoveTemp(Callback));
  else
    return FECFHandle();
}
```

It is done! Now you can run your own action:

```cpp
FFlow::NewAction(this, 1, 2, [this](bool bStopped)
{
  // Callback code.
}, ECF_IGNOREPAUSE);
```

## Disabling build optimization
You can temporarily disable plugin's build optimizations by setting the `bDisableOptimization` parameter in `EnhancedCodeFlow.Build.cs` file to `true`. It can help with debugging.

[Back to top](#table-of-content)

# Extra links

- [Unreal Engine Forum](https://forums.unrealengine.com/t/plugin-enhanced-code-flow/684237)
- [Blog Entry](https://zompidev.blogspot.com/2021/03/enhanced-code-flow-plugin-for-ue4.html)

[Back to top](#table-of-content)

# Special thanks

I want to send special thanks to Monika, because she always supports me and believes in me, to Pawel, for allowing me to test this plugin on his project and to everyone that contributed to this project.  
Also, I want to thank You for using this plugin! It is very important for me that my work is useful for someone!  
Happy coding!

[Back to top](#table-of-content)
