# Dynamic Casting and Multithreading Example in C++

This program demonstrates the use of dynamic casting and multithreading in C++. It simulates the management of multiple devices working in the background, logging messages while active. The devices run independently in separate threads, and the program ensures proper resource management and thread safety.

---

## **Key Points**

### **1. API Overview**
The program defines a basic API for device management:
- **`IDevice` Interface:** Acts as a generic interface for devices.
- **API Functions:**
  - `createDevice()`: Creates a device instance.
  - `startDevice(IDevice* device)`: Starts a device.
  - `stopDevice(IDevice* device)`: Stops a device.
  - `destroyDevice(IDevice* device)`: Destroys a device.

### **2. Internal Implementation**
- **Class Structure:**
  - `IBaseDevice`: Abstract class with `start()` and `stop()` methods.
  - `Device`: Implements `IBaseDevice` and inherits from `IDevice`.
- **Attributes in `Device`:**
  - `std::atomic<bool> running`: Tracks the running state.
  - `std::thread worker`: Background thread for device work.
  - `int counter_`: Simulates work progress.

- **Key Methods in `Device`:**
  - `start()`: Launches the worker thread.
  - `stop()`: Stops the thread and ensures it joins properly.
  - `run()`: Contains the thread’s work logic, logs messages, and increments the counter.
  - `generateRandomNumberOfSeconds()`: Simulates processing delays.

### **3. Thread-Safe Logging**
- **Callback Setup:** `initLogging(void(*logCallback)(const char*))` initializes a callback for logging.
- **Thread Safety:** Uses a `std::mutex` to ensure safe logging from multiple threads.

### **4. Main Program Workflow**
1. **Initialize Logging:** Defines a callback to print log messages.
2. **Create Devices:** Initializes 5 devices and stores them in a vector.
3. **Start Devices:** Launches each device’s worker thread.
4. **Simulate Work:** Allows devices to log messages for 5 seconds.
5. **Stop Devices:** Stops and joins all threads.
6. **Destroy Devices:** Cleans up resources and clears the vector.

---

## **Summary**
This example highlights:
- **Dynamic Casting:** Used to convert between interfaces and concrete classes.
- **Multithreading:** Devices work independently in separate threads.
- **Thread Safety:** Managed with `std::atomic` and `std::mutex`.
- **Resource Management:** Ensures proper cleanup of threads and objects.

The program serves as a practical demonstration of advanced C++ features like multithreading, dynamic casting, and thread-safe logging.

## How to compile (on linux using g++).

```
g++ main.cpp -o main
```
## Output example:

```
$ ./main
I am 140008371127872 (0x7f563d3a0640) device trabajando. Counter=1
I am 140008379582016 (0x7f563dbb0640) device trabajando. Counter=1
I am 140008362673728 (0x7f563cb90640) device trabajando. Counter=1
I am 140008388036160 (0x7f563e3c0640) device trabajando. Counter=1
I am 140008283375168 (0x7f5637ff0640) device trabajando. Counter=1
I am 140008371127872 (0x7f563d3a0640) device trabajando. Counter=2
I am 140008283375168 (0x7f5637ff0640) device trabajando. Counter=2
I am 140008379582016 (0x7f563dbb0640) device trabajando. Counter=2
I am 140008362673728 (0x7f563cb90640) device trabajando. Counter=2
I am 140008388036160 (0x7f563e3c0640) device trabajando. Counter=2
I am 140008371127872 (0x7f563d3a0640) device trabajando. Counter=3
I am 140008283375168 (0x7f5637ff0640) device trabajando. Counter=3
I am 140008388036160 (0x7f563e3c0640) device trabajando. Counter=3
I am 140008379582016 (0x7f563dbb0640) device trabajando. Counter=3
I am 140008371127872 (0x7f563d3a0640) device trabajando. Counter=4
I am 140008362673728 (0x7f563cb90640) device trabajando. Counter=3
.
.
(more traces)
.
.
I am 140008388036160 (0x7f563e3c0640) device trabajando. Counter=25
I am 140008283375168 (0x7f5637ff0640) device trabajando. Counter=27
I am 140008371127872 (0x7f563d3a0640) device trabajando. Counter=26
I am 140008362673728 (0x7f563cb90640) device trabajando. Counter=28
I am 140008379582016 (0x7f563dbb0640) device trabajando. Counter=27
I am 140008388036160 (0x7f563e3c0640) device trabajando. Counter=26
I am 140008362673728 (0x7f563cb90640) device trabajando. Counter=29
I am 140008283375168 (0x7f5637ff0640) device trabajando. Counter=28
I am 140008371127872 (0x7f563d3a0640) device trabajando. Counter=27
I am 140008379582016 (0x7f563dbb0640) device trabajando. Counter=28
Stopped thread: 140008388036160
I am 140008283375168 (0x7f5637ff0640) device trabajando. Counter=29
I am 140008371127872 (0x7f563d3a0640) device trabajando. Counter=28
I am 140008362673728 (0x7f563cb90640) device trabajando. Counter=30
Stopped thread: 140008379582016
I am 140008283375168 (0x7f5637ff0640) device trabajando. Counter=30
Stopped thread: 140008371127872
Stopped thread: 140008362673728
Stopped thread: 140008283375168
$

```