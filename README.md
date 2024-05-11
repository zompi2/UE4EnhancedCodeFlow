# Enhanced Code Flow for Unreal Engine

This code plugin provides functions that drastically improve the quality of life during the implementation of game flow in C++.  
It works very well with gameplay programming, UI programming with a lot of transitions or in any other situation.

The plugin has been tested on the Engine's versions: 4.27.2, 5.3.1 and 5.4.0.

# Table of content

- [Contact](#contact)
- [Unreal Engine Marketplace](#unreal-engine-marketplace)
- [Changelog](#changelog)
- [LEGACY VERSIONS](#legacy-versions)
- [Example Project](#example-project)
- [Used in...](#used-in)
---
- [Installation](#installation)
- [Usage](#usage)
- [Extra Settings](#extra-settings)
- [Instanced Actions](#instanced-actions)
- [Coroutines (experimental)](#coroutines-experimental)
- [Pausing and Resuming](#pausing-and-resuming)
- [Stopping Actions](#stopping-actions)
- [Measuring Performance](#measuring-performance)
- [Extending Plugin](#extending-plugin)
- [Extra Links](#extra-links)
- [Special Thanks](#special-thanks)

# Contact

If you have any question or suggestion regardles this plugin simply add an **Issue** to the github project or write an e-mail to me: **zompi2@gmail.com** I will try my best to answer it quickly :)

[Back to top](#table-of-content)

# Unreal Engine Marketplace

The plugin is finally available on the Unreal Engine Marketplace! It is free, of course.  
If you don't want to build this plugin by yourself, you can **[download it from here](https://www.unrealengine.com/marketplace/en-US/product/enhanced-code-flow)**.  
Unreal Engine Marketplace allow to publish the code plugins for new versions of the Unreal Engine only, so currently only 5.4, 5.3 and 5.2 are available.  
If you are using 4.27 you can download the precompiled package **[from here](https://github.com/zompi2/UE4EnhancedCodeFlow/raw/build-4.27/EnhancedCodeFlow-4.27-Compiled.zip)**.  
The plugin's version that's on the Marketplace is **3.3.2**.

[Back to top](#table-of-content)

# Changelog

The Changelog has been put into this file: **[Changelog.txt](Changelog.txt)**

[Back to top](#table-of-content)

## **LEGACY VERSIONS**  

Version `3.1.1` was the last one that had an example project implemented in UE4. Next versions' example projects require UE5 to run.  
Version `3.1.1` can be found on a separate branch here: **[Legacy-3.1.1](https://github.com/zompi2/UE4EnhancedCodeFlow/tree/Legacy-3.1.1)**  

Version `3.0.0` will probably break code and Blueprint nodes from previous version. Update with caution!  
Version `2.1.2` can be found on a separate branch here: **[Legacy-2.1](https://github.com/zompi2/UE4EnhancedCodeFlow/tree/Legacy-2.1)**  

Version `2.0.0` will probably break Blueprint nodes from previous versions. Update with caution!  
Version `1.6.1` can be found on a separate branch here: **[Legacy-1.6](https://github.com/zompi2/UE4EnhancedCodeFlow/tree/Legacy-1.6)**

[Back to top](#table-of-content)

# Example Project

The example project wich uses this plugin can be found in **[this repository](https://github.com/zompi2/UE4EnhancedCodeFlowExample)**. Example project is compatible with the newest version of the plugin only.

> !!!IMPORTANT!!!  
> Currently Example Project will work with **Unreal Engine 5**! The last version of the example project that can be run on UE4 can be found **[here](https://github.com/zompi2/UE4EnhancedCodeFlowExample/tree/Legacy-3.1.1)**. This is a legacy example project which works with ECF 3.1.1. It is not guaranteed that it will work with the newest version of ECF.

![Main](https://github.com/zompi2/UE4EnhancedCodeFlow/assets/7863125/87bf7f3f-2db6-42d5-9195-208a401d84d9)

[Back to top](#table-of-content)

# Used In

Enhanced Code Flow has been used in the following projects:  
- [Achilles: Legends Untold](https://store.steampowered.com/app/1314000/Achilles_Legends_Untold)
- [Wanderer's Sigil](https://store.steampowered.com/app/2436870/Wanderers_Sigil)
- [Blaze in Space: Beat a-Maze](https://store.steampowered.com/app/2016110/Blaze_in_Space_Beat_aMaze)
- [The Neon Hook](https://zompi.itch.io/the-neon-hook)

If you are using ECF in your project, let me know :)

[Back to top](#table-of-content)

# Installation

1. Get ECF plugin. You can do this by either:
    * Getting if from the Epic Games Launcher. It will be installed in the Engine's directory: `Engine/Plugins/Marketplace/EnhancedCodeFlow`.
    * Cloning it or downloading it from this repository and putting it into your project's (`MyProject/Plugins/EnhancedCodeFlow`) or engine's (`Engine/Plugins/EnhancedCodeFlow`) plugins directory.
2. Add "EnhancedCodeFlow" entry to the `PublicDependencyModuleNames` list in your project's `.Build.cs` file.
3. Enable the plugin in the Editor's Plugins manager (or by changing manually your project's `.uproject` file).
4. If you want to use this plugin in your code add `#include "EnhancedCodeFlow.h` to the file in which you want to use the plugin.

Check out the **[Example Project](https://github.com/zompi2/UE4EnhancedCodeFlowExample)** to see how the plugin is integrated into it.

[Back to top](#table-of-content)

# Usage

- [Delay](#delay)
- [Delay Ticks](#delay-ticks)
- [Add Ticker](#add-ticker)
- [Wait And Execute](#wait-and-execute)
- [While True Execute](#while-true-execute)
- [Run Async Then](#run-async-then)
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

#### Delay Ticks

Execute specified action after some ticks. Can be useful if we want to execute some code in next game tick.

``` cpp
FFlow::DelayTicks(this, 1, [this](bool bStopped)
{
  // Code to execute after 1 tick.
});
```

![ecfticks](https://github.com/zompi2/UE4EnhancedCodeFlow/assets/7863125/39d1b743-f373-435c-befc-290c90dd720c)

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
FFlow::WaitAndExecute(this, [this](float DeltaTime)
{
  // Write your own predicate. 
  // Return true when you want to execute the code below.
  // The DeltaTime parameter is optional.
  return bIsReadyToUse;
},
[this](bool bTimedOut, bool bStopped)
{
  // Implement code to execute when conditions are met or when this action has ran for 5 seconds (time specified in a timeout parameter)
}, 5.f);
```

BP version of this function uses a `Predicate` function which controls when the `On Execution` pin will execute.

![WaitAndExecute](https://github.com/zompi2/UE4EnhancedCodeFlow/assets/7863125/882f7637-5f2a-4e7d-b0ef-093da3693a33)

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

![WhileTrueExecute](https://github.com/zompi2/UE4EnhancedCodeFlow/assets/7863125/999b064d-9ea9-4a15-9998-8c15bbd10ff0)

[Back to actions list](#usage)  
[Back to top](#table-of-content)

#### Run Async Then

Runs the given task function on a separate thread and calls the callback function when this task ends.
You can specify a timeout, which will stop this action after the given time.  

> Have in mind, that the neither the timeout nor stopping the action will not stop the running async thread. It just won't trigger the callback when the async task ends. Handle timeout on the side of the async task itself.  

The `bStopped` tells if this action has been stopped by a Stop function.  
You can define the priority of the running task as `Normal` (`AnyBackgroundThreadNormalTask`) or `HiPriority` (`AnyBackgroundHiPriTask`).

> Have in mind, that you can start this function from GameThread only!


``` cpp
FFlow::RunAsyncThen(this, [this]()
{
  // This code runs on the background thread.
},
[this](bool bTimedOut, bool bStopped)
{
  // This code runs on a game thread after the previous block of code finishes it's run.
}, 0.f, EECFAsyncPrio::Normal);
```

The BP node exists for this function, but have in mind that Unreal does not allow for many non-gamethread operations in Blueprints! Use this node with caution!

![runathen](https://github.com/zompi2/UE4EnhancedCodeFlow/assets/7863125/ff9c423e-7a8f-4c33-af6e-d860f3940d82)

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

# Coroutines (experimental)

> Coroutines are treated as an **experimental** feature. You can use them at your own risk!

[Coroutines](https://en.cppreference.com/w/cpp/language/coroutines) are functions that can suspend their execution and be resumed later. They require C++20 which is supported in Unreal Engine from verion 5.3. To make sure that your project supports C++20 add the following line to your project's `Build.cs`:

``` cs
CppStandard = CppStandardVersion.Cpp20;
```

Every coroutine must return the `FECFCoroutine`. ECF implements some helpful coroutines described below. Every coroutine implemented in ECF works simillar to typical ECF action, but they use the coroutine suspension mechanisms instead of lambdas.  
They can be paused, resumed, cancelled and they can accept `FECFActionSettings`.  
Coroutines doesn't have BP nodes as they are purely code feature.

- [Wait Seconds](#wait-seconds)
- [Wait Ticks](#wait-ticks)
- [Wait Until](#wait-until)
- [Run Async And Wait](#run-async-and-wait)

[Back to top](#table-of-content)

#### Wait Seconds

Suspends the coroutine for a specified amount of seconds. It works like Delay in Blueprints.

``` cpp
FECFCoroutine UMyClass::SuspandableFunction()
{
  // Do something
  co_await FFlow::WaitSeconds(this, 2.f);
  // Do something after 2 seconds
}
```

[Back to coroutines](#coroutines-experimental)  
[Back to top](#table-of-content)

#### Wait Ticks

Suspends the coroutine for a specified amount of tick.

``` cpp
FECFCoroutine UMyClass::SuspandableFunction()
{
  // Do something
  co_await FFlow::WaitTicks(this, 1);
  // Do something after 1 tick
}
```

[Back to coroutines](#coroutines-experimental)  
[Back to top](#table-of-content)

#### Wait Until

Suspends the coroutine until the given predicate conditions are met.

``` cpp
FECFCoroutine UMyClass::SuspandableFunction()
{
  // Do something
  co_await FFlow::WaitUntil(this, [this](float DeltaTime)
  {
    // Write your own predicate. 
    // Return true when you want to resume the coroutine function.
    return bIsReadyToUse;
  }, TimeOut);
  // Do something after conditions specified in the predicate are met. 
}
```

[Back to coroutines](#coroutines-experimental)  
[Back to top](#table-of-content)

#### Run Async And Wait

Runs the given block of code on a background thread and wait for it's completion before moving on.
> Have in mind, that you can start this coroutine from GameThread only!

``` cpp
FECFCoroutine UMyClass::SuspandableFunction()
{
  // Do something
  co_await FFlow::RunAsyncAndWait(this, [this]()
  {
    // This code will run on a separate background thread.
  }, TimeOut, EECFAsyncPrio::Normal);
  // Do something after the above code has finished.
}
```

[Back to coroutines](#coroutines-experimental)  
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
FFlow::RemoveAllRunAsyncThen(GetWorld());
FFlow::RemoveAllTimelines(GetWorld());
FFlow::RemoveAllCustomTimelines(GetWorld());
FFlow::RemoveAllTimeLocks(GetWorld());
FFlow::RemoveAllDoNoMoreThanXTimes(GetWorld());
```

![allremove](https://github.com/zompi2/UE4EnhancedCodeFlow/assets/7863125/933b9695-02ff-402d-947d-ac613b4dc14f)

You can also stop all of the running actions that handle coroutines.

``` cpp
FFlow::RemoveAllWaitSeconds(GetWorld(), true);
FFlow::RemoveAllWaitTicks(GetWorld(), true);
FFlow::RemoveAllWaitUntil(GetWorld(), true);
FFlow::RemoveAllRunAsyncAndWait(GetWorld(), true);
```

**IMPORTANT!** If you stop the action which handles a coroutine be aware that if you won't set `bComplete` to true, the suspended coroutine will never be resumed!

[Back to top](#table-of-content)

# Measuring Performance

## Stats

To measure plugin's performance you can use the stat command designed for it: `stat ecf`  
![stat](https://github.com/zompi2/UE4EnhancedCodeFlow/assets/7863125/ccd5ab03-d1b1-423a-837d-cdd828605508)
There are the following stats:  
* Tick - the time in `ms` the plugin needs to perform one full update.  
* Actions - the amount of actions that are currently running.
* Instances - describes how many of the running actions are the instanced ones.
* Action Objects - the amount of the real action UObjects residing in the memory.
* Async BP Objects - the amount of the real UObjects handling async BP calls residenting in the memory.

> Have in mind that `Action Objects` and `Async BP Objects` counts Class Default Objects (CDO) too, so their values will never be 0.

> `Tick` measures not only the time the plugin needs to do it's job, but also the time needed to run the code inside action's callbacks.

You can also display more detailed plugin's performance info with `stat ecfdetails`. It will display the time needed by every type of action.

![image](https://github.com/zompi2/UE4EnhancedCodeFlow/assets/7863125/98e13279-52ba-424f-9100-1e1405ed04f3)

> Have in mind that custom tick intervals might alter the values of stats, especially the `CallCounts`.

## Unreal Insights

You can measure performence using [Unreal Insights](https://docs.unrealengine.com/4.26/en-US/TestingAndOptimization/PerformanceAndProfiling/UnrealInsights/) tool. Currently there is only one cpu timing trace available:
* ECF-Actions-Tick - measures the time the plugin needs to perform one full update.

You can disable Unreal Insights traces for this plugin by setting `bEnableInsightProfiling` in `EnhancedCodeFlow.Build.cs` to `false`.

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
9. You can optionally add the stats counter to your action's `Tick` function, in order to measure it's performence  with `stat ecfdetails`.
```cpp
DECLARE_SCOPE_CYCLE_COUNTER(TEXT("NewAction - Tick"), STAT_ECFDETAILS_NEWACTION, STATGROUP_ECFDETAILS);
```
It is done! Now you can run your own action:

```cpp
FFlow::NewAction(this, 1, 2, [this](bool bStopped)
{
  // Callback code.
}, ECF_IGNOREPAUSE);
```

### Adding coroutines

Adding actions that supports coroutine is simillar to adding new actions but there are few steps that must be made:

1. Action class must inherits from `UECFCoroutineActionBase` instead of `UECFActionBase`.
2. Add coroutine task to `ECFCoroutineTasks.h`
```cpp
class ENHANCEDCODEFLOW_API FECFCoroutineTask_NewCoroAction : public FECFCoroutineTask
{
public:

	FECFCoroutineTask_NewCoroAction(const UObject* InOwner, const FECFActionSettings& InSettings, int32 InParam1);
	void await_suspend(FECFCoroutineHandle CoroHandle);

private:

	int32 Param1 = 0;
};
```
3. Implement coroutine task in `ECFCoroutineTasks.cpp`. Use `AddCoroutineAction`` function there.
```cpp
FECFCoroutineTask_NewCoroAction::FECFCoroutineTask_NewCoroAction(const UObject* InOwner, const FECFActionSettings& InSettings, int32 InParam1)
{
	Owner = InOwner;
	Settings = InSettings;
	Param1 = InParam1;
}

void FECFCoroutineTask_NewCoroAction::await_suspend(FECFCoroutineHandle CoroHandle)
{
	AddCoroutineAction<UNewCoroAction>(Owner, CoroHandle, Settings, Param1);
}
```
4. Coroutine action implementation must resume coroutine in `Complete` function:
```cpp
void Complete(bool bStopped) override
{
	CoroutineHandle.resume();
}
```
5. The coroutine should be called from `FEnhancedCodeFlow` class and it's implementation should return the defined coroutine task:
```cpp
FECFCoroutineTask_NewCoroAction FEnhancedCodeFlow::NewCoroAction(const UObject* InOwner, int32 InParam1, const FECFActionSettings& Settings)
{
	return FECFCoroutineTask_NewCoroAction(InOwner, Settings, InParam1);
}
```

[Back to top](#table-of-content)

## Disabling build optimization
You can temporarily disable plugin's build optimizations by setting the `bDisableOptimization` parameter in `EnhancedCodeFlow.Build.cs` file to `true`. It can help with debugging.

[Back to top](#table-of-content)

# Extra links

* [Unreal Engine Forum](https://forums.unrealengine.com/t/plugin-enhanced-code-flow/684237)
* [Blog Entry](https://zompidev.blogspot.com/2021/03/enhanced-code-flow-plugin-for-ue4.html)
* [Reddit Post](https://www.reddit.com/r/unrealengine/comments/mbkzqj/enhanced_code_flow_plugin)

[Back to top](#table-of-content)

# Special thanks

I want to send special thanks to Monika, because she always supports me and believes in me, to Pawel, for allowing me to test this plugin on his project and to everyone that contributed to this project.  
Also, I want to thank You for using this plugin! It is very important for me that my work is useful for someone!  
Happy coding!

[Back to top](#table-of-content)
