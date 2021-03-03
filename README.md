# Enhanced Code Flow for Unreal Engine 4

The goal of this plugin is to provide functionalities that can improve implementation of code flow.  
These are e.g. launching code when specific conditions are made, or running descrete timelines.  
All functions are described below, in this readme file.

### Installation

This is a code only plugin. It means that to use it you have to:
1. Put the whole plugin into a Plugins directory in your project
2. Add **"EnhancedCodeFlow"** entry into **PublicDependencyModuleNames** in your project's **build.cs** file.
3. Add **#include "EnhancedCodeFlow.h"** to a file in which you want to use the plugin

# Usage


Run the following functions to implement enhanced code flow into your project!

> Note that every function must receive a pointer to an owner that runs this function in it's first argument.  
> The owner must be able to return a World via **GetWorld()** function.

#### Delay

It will run the block of code after a delay specified in seconds. This can be useful in many various situations. Everytime when I was using a Delay node
in blueprints I wish there was an equivalent of it in c++.

``` cpp
FFlow::Delay(this, 2.f, [this]()
{
	// Code to execute after 2 seconds.
});
```

#### Add Ticker

Creates a discrete ticker. You can define how long it can tick or run it infinitelly and stop it explicitly.  
Useful for actors and components that you don't want to be tickeable, but needs one tick to do something.

**Run ticker for 10 seconds**

``` cpp
FFlow::AddTicker(this, 10.f, [this](float DeltaTime)
{
	// Code to execute every tick
});
```

**Run ticker for infinite and stop it when you want to**

``` cpp
FFlow::AddTicker(this, [this](float DeltaTime, FECFHandle TickerHandle)
{
	// Code to execute in every tick.

	// Use this to stop the ticker
	FFlow::StopAction(this, TickerHandle);
});
```

**Run ticker for infinite and someone else stops it**

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

Waits until specific conditions are made and then executes code. The conditions are defined in a form of predicate. It is useful in situations when you
are not sure if the object you want to use is already spawned.

``` cpp
FFlow::WaitAndExecute(this, [this]()
{
	// Write your own predicate. Return true when you want to execute the code below.
	return bIsReadyToUse;
},
[this]()
{
	// Implement code to execute when conditions are met.
});
```

#### While true execute

While the specified conditions are true tick the given code. This one is useful when you want to write a while loop that executes one run every tick. Can be used as a substitute of coroutines.

``` cpp
FFlow::WhileTrueExecute(this, [this]()
{
	// Write your own predicate. Return true when you want this action to continue.
	return bIsRunning;
},
[this](float DeltaTime)
{
	// Implement code to tick when conditions are true.
});
```

#### Add timelie

Creates a descrete timeline during which the code can be implemented. Useful when creating any sort of blends.  
The function requires the following parameters:  
* StartValue - a value with which the timeline will begin;
* StopValue - a value with which the timeline will end. StopValue can be lesser thant StartValue;
* Time - how long the timeline will work;
* TickFunc - a function that will tick with the timeline. It has the following arguments:
  * Value - a current value on this timelnie;
  * Time - a time that passed on this timeline;
* CallbackFunc - a funcion that will run when timeline comes to an end. Has the same arguments as TickFunc. This function is *optional*;
* BlendFunc - a function that describes the shape of a timeline:
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

#### Add custom timelie

Creates a descrete timeline which shape is based on a **UCurveFloat**. Works like the previously described timeline, but an asset with a curve must be given.

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

# Stopping actions

Every function described earlier returns a **FECFHandle** which can be used to check if the following action is running and to stop it.

``` cpp
FFlow::IsActionRunning(GetWorld(), Handle); // <- checks if the action is running
FFlow::StopAction(GetWorld(), Handle); // <- stops the action
```

> Note that this function requires a pointer to the existing **World** in order to work properly.

You can also stop all of the actions from a specific owner or from everywhere:

``` cpp
FFlow::StopAllActions(GetWorld()); // <- stops all of the actions
FFlow::StopAllActions(GetWorld(), Owner); // <- stops all of the actions started from this specific owner
```

You can also stop all of the specific actions. In this case you can also optionally specifiy an owner of this actions, or simply stop all of them.

``` cpp
FFlow::RemoveAllDelays(GetWorld());
FFlow::RemoveAllTickers(GetWorld());
FFlow::RemoveAllWaitAndExecutes(GetWorld());
FFlow::RemoveAllWhileTrueExecutes(GetWorld());
FFlow::RemoveAllTimelines(GetWorld());
FFlow::RemoveAllCustomTimelines(GetWorld());
```

# Extending plugin

If you have a source code of this plugin it can be easily extended! Here's how to add your own custom action to the plugin.

[BIG TODO]