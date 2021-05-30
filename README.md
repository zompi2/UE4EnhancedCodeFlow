# Enhanced Code Flow for Unreal Engine 4

This code plugin provides functions that drastically improve the quality of life during the implementation of game flow in C++.  
It works very well with gameplay programming, UI programming with a lot of transitions or in any other situation.

### UE5

This plugin works with Unreal Engine 5 too! Checkout branch **ue5** to get a compatible version. 

# Contact

If you have any question or suggestion regardles this plugin simply add an **Issue** to the github project or write an e-mail to me: **zompi2@gmail.com** I will try my best to answer it quickly :)

# Changelog

The Changelog has been put into this file: **[Changelog.txt](Changelog.txt)**

# Example Project

The example project wich uses this plugin can be found in **[this repository](https://github.com/zompi2/UE4EnhancedCodeFlowExample)**.

![ecf](https://user-images.githubusercontent.com/7863125/111898879-0d845280-8a29-11eb-82cb-2953b4069b26.png)

# Usage

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

#### Wait and execute

Waits until specific conditions are made and then executes code.  
The conditions are defined in a form of predicate.  
Perfect solution if code needs a reference to an object, which spawn moment is not clearly defined. 

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

#### While true execute

While the specified conditions are true tick the given code.  
This one is useful when you want to write a loop that executes one run every tick until it finishes it's job.  

``` cpp
FFlow::WhileTrueExecute(this, [this]()
{
  // Write your own predicate. 
  //Return true when you want this action to continue.
  return bIsRunning;
},
[this](float DeltaTime)
{
  // Implement code to tick when conditions are true.
});
```

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

# Extra settings

You can define extra settings at the end of each action launch. Currently the following actions are available:
* Time Intervals - defines the length of one tick.
* First Delay - defines when the first tick should be performed.
* Ignore Game Pause - it will ignore the game pause.
* Ignore Global Time Dilation - it will ignore global time dilation when ticking.

``` cpp
FFlow::AddTicker(this, 10.f, [this](float DeltaTime)
{
  // Code to execute every 1 second for 10 seconds.
}, nullptr, FECFActionSettings(1.f, 0.f, false, false));
```

``` cpp
FFlow::AddTicker(this, 10.f, [this](float DeltaTime)
{
  // Code to execute every tick for 10 seconds 
  // while ignoring global time dilation.
}, nullptr, FECFActionSettings(0.f, 0.f, true, false));
```

``` cpp
FFlow::AddTicker(this, 10.f, [this](float DeltaTime)
{
  // Code to execute every 1 seconds for 10 seconds, 
  // after 5 seconds have passed, while ignoring 
  // global time dilation and pause.
}, nullptr, FECFActionSettings(1.f, 5.f, true, true));
```

To make defining these settings easier there are few macros that creates a settings structure with just one option:

* `ECF_TICKINTERVAL(5.f)` - settings which sets tick interval to 5 second
* `ECF_DELAYFIRST(1.f)` - settings which makes this action to run after 1 second delay
* `ECF_IGNOREPAUSE` - settings which makes this action ignore game pause
* `ECF_IGNORETIMEDILATION` - settings which makes this action ignore global time dilation
* `ECF_IGNOREPAUSEDILATION` - settings which makes this action ignore pause and global time dilation

``` cpp
FFlow::Delay(this, 2.f, [this]()
{
  // Run this code after 2 seconds, while ignoring game pause.
}, ECF_IGNOREPAUSE);
```

# Stopping actions

Every function described earlier returns a **FECFHandle** which can be used to check if the following action is running and to stop it.

``` cpp
FFlow::IsActionRunning(GetWorld(), Handle); // <- is this action running?
FFlow::StopAction(GetWorld(), Handle); // <- stops this action!
```

> Note that this function requires a pointer to the existing **World** in order to work properly.

You can also stop all of the actions from a specific owner or from everywhere. Stopped actions can launch their completion callbacks or not, depending on the given argument:

``` cpp
FFlow::StopAllActions(GetWorld()); // <- stops all of the actions
FFlow::StopAllActions(GetWorld(), true); // <- stops all of the actions and launch their callbacks
FFlow::StopAllActions(GetWorld(), false, Owner); // <- stops all of the actions started from this specific owner
```

You can also stop all of the **specific** actions. In this case you can also optionally specifiy an owner of this actions, or simply stop all of them.
You can also specify if stopped actions should launch their completion callbacks or not.

``` cpp
FFlow::RemoveAllDelays(GetWorld());
FFlow::RemoveAllTickers(GetWorld());
FFlow::RemoveAllWaitAndExecutes(GetWorld());
FFlow::RemoveAllWhileTrueExecutes(GetWorld());
FFlow::RemoveAllTimelines(GetWorld());
FFlow::RemoveAllCustomTimelines(GetWorld());
```

# Extending plugin

If you have a source code of this plugin you can easily extend it's functionalities!

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
6. In the `FEnhancedCodeFlow` class implement static function that launches the action using `AddAction` function.
   The function must receive a pointer to the launching `UObject`, `FECFActionSettings` structure and every other argument that is used in the action's `Setup` function in the same order.
   It must return `FECFHandle`.
```cpp
FECFHandle FEnhancedCodeFlow::NewAction(UObject* InOwner, int32 Param1, int32 Param2, TUniqueFunction<void()>&& Call, const FECFActionSettings& Settings = {})
{
  if (UECFSubsystem* ECF = UECFSubsystem::Get(InOwner))
    return ECF->AddAction<UECFNewAction>(InOwner, Settings, Param1, Param2, MoveTemp(Call));
  else
    return FECFHandle();
}
```
7. You can optionally add static function which will stop this action
```cpp
void FFlow::RemoveNewActions(const UObject* WorldContextObject, bool bComplete, UObject* InOwner)
{
  if (UECFSubsystem* ECF = UECFSubsystem::Get(InOwner))
  {
    ECF->RemoveActionsOfClass<UECFNewAction>(bComplete, InOwner);
  }
}
```
8. You can optionally run `SetMaxActionTime` in actions `Init` phase to determine the maximum time in seconds this action should run. 
>IMMPORTANT! SetMaxActionTime is only to help ticker run ticks with proper delta times.  
   >It will not stop the action itself!

It is done! Now you can run your own action:

```cpp
FFlow::NewAction(this, 1, 2, [this]()
{
  // Callback code.
}, ECF_IGNOREPAUSE);
```

# Blueprints support

Even though this was originally code only plugin I decided to move it's functions to Blueprints too:

#### Delay

![delaynode](https://user-images.githubusercontent.com/7863125/117880380-5679ba00-b2a8-11eb-9a10-e68e1a0e27e3.png)

#### Add Ticker

![ticker](https://user-images.githubusercontent.com/7863125/117880473-727d5b80-b2a8-11eb-8c5f-31c34a65abbc.png)

#### Wait and Execute

![wee](https://user-images.githubusercontent.com/7863125/117880636-a0fb3680-b2a8-11eb-9bce-76282855d969.png)

#### While True Execute

![wte](https://user-images.githubusercontent.com/7863125/117880678-aeb0bc00-b2a8-11eb-8bd6-3bbed4199d0e.png)

#### Add Timeline

![timel](https://user-images.githubusercontent.com/7863125/117880748-bec89b80-b2a8-11eb-95e7-bf96e5626c5b.png)

#### Add Custom Timeline

![cust](https://user-images.githubusercontent.com/7863125/117880805-cd16b780-b2a8-11eb-9a9f-4f5400a21a05.png)

#### Other functions

![newbp](https://user-images.githubusercontent.com/7863125/118320999-44dd2000-b4fd-11eb-8081-f280067c0f85.png)

# Special thanks

I want to send special thanks to Monika, because she always supports me and believes in me, to Pawel, for allowing me to test this plugin on his project and to everyone that contributed to this project.  
Also, I want to thank You for using this plugin! It is very important for me that my work is useful for someone!  
Happy coding!
