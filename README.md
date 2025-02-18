# **QNX Drone Control System**

## **Overview**
QNX Drone Control System is an embedded real-time control system designed for drones, leveraging **QNX RTOS** for deterministic and high-reliability operations. The system includes joystick input handling, network communication, video streaming, and real-time scheduling mechanisms, making it suitable for applications requiring precise and responsive control.

## **Features**
✅ **Real-time Processing** – Utilizes QNX RTOS to ensure low-latency and deterministic execution.  
✅ **Joystick Input Handling** – Supports external controllers for drone navigation and command input.  
✅ **Network Communication** – Implements socket-based communication for control and telemetry data exchange.  
✅ **Streaming Capability** – Supports video or telemetry streaming using a client-server model.  
✅ **Timer-based Scheduling** – Uses real-time timers for precise task scheduling and execution.  
✅ **Modular Design** – Separated components for easy extension and maintenance.  

## **Project Structure**
```bash
QNX-Drone-Control-System/ 
│── drone/ 
│ ├── cjoystick.cpp/h # Joystick input handling
│ ├── cqstreamclient.cpp/h # Streaming client
│ ├── cqstreamsrv.cpp/h # Streaming server
│ ├── csocket.cpp/h # Network communication
│ ├── ctimer.cpp/h # Real-time timers
│ ├── drone.cc # Main control program
│ ├── Makefile # Build script
│ ├── x86/ # Compiled binaries (if any)
│── vm/ # Virtual machine configurations
│── .metadata/ # Eclipse CDT configuration files
│── log.txt # Debug logs
```

## **Installation**

### **Prerequisites**
- **QNX SDP (Software Development Platform)**
- **QNX Momentics IDE** (Recommended for development)
- **GCC or QNX Neutrino compiler**  
- **Hardware**: A drone with QNX support or a QNX virtual machine  

### **Building the Project**
1. Clone the repository:
   ```bash
   git clone https://github.com/Pocket99/QNX-Drone-Control-System.git
   cd QNX-Drone-Control-System/drone
   ```
2. Compile the source code:
  ```bash
  make
  ```
3. Deploy the binary to your QNX system.

### **Usage**
1. Start the control system
```bash
./drone
```
2. Connect a joystick (if supported).
3. Monitor logs for real-time debugging:
```bash
tail -f log.txt
```
4. Network Communication: Ensure the drone control system is reachable via the configured network socket.
