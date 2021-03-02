# Enhanced Code Flow for Unreal Engine 4

The goal of this plugin is to provide functionalities that can improve implementation of code flow.  
These are e.g. launching code when specific conditions are made, or running descrete timelines.  
All functions are described below, in this readme file.

### Installation

This is a code only plugin. It means that to use it you have to:
1. Put the whole plugin into a Plugins directory in your project
2. Add **"EnhancedCodeFlow"** entry into **PublicDependencyModuleNames** in your project's **build.cs** file.
3. Add **#include "EnhancedCodeFlow.h"** to a file in which you want to use the plugin

### Usage

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

##### Run ticker for 10 seconds

``` cpp
FFlow::AddTicker(this, 10.f, [this](float DeltaTime)
{
	// Code to execute every tick
});
```

##### Run ticker for infinite and stop it when you want to

``` cpp
FFlow::AddTicker(this, [this](float DeltaTime, FECFHandle TickerHandle)
{
	// Code to execute in every tick.

	// Use this to stop the ticker
	FFlow::StopAction(this, TickerHandle);
});
```

##### Run ticker for infinite and someone else stops it

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

