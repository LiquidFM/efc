project (efc)

# Project header
project_header_default ("POSITION_INDEPENDENT_CODE:YES")

# 3rdparty
find_package (Threads REQUIRED)

# Sources
add_subdirectory (src)

# Target
add_library (efc STATIC ${${PROJECT_NAME}_SOURCES})
target_link_libraries (efc brolly ${CMAKE_THREAD_LIBS_INIT})
add_dependencies (efc platform)

# Documentation
add_documentation (efc 0.0.1 "Exceptions Free Containers library")

# Install rules
install_header_files (efc "src/efc_move.h:move"
                          "src/efc_list.h:List"
                          "src/efc_map.h:Map"
                          "src/efc_set.h:Set"
                          "src/efc_string.h:String"
                          "src/efc_type_traits.h:type_traits"
                          "src/efc_vector.h:Vector"
                          "src/atomics/efc_atomics.h:Atomics"
                          "src/efc_holder.h:Holder"
                          "src/efc_pair.h:Pair"
                          "src/efc_memory.h:ScopedPointer"
                          "src/efc_memory.h:SharedPointer"
                          "src/efc_statemachine.h:StateMachine"
                          "src/efc_staticarray.h:StaticArray"
                          "src/efc_variant.h:Variant"
                          "src/loki-0.1.7/NullType.h"
                          "src/loki-0.1.7/Typelist.h"
                          "src/loki-0.1.7/TypelistMacros.h"
                          "src/loki-0.1.7/TypeManip.h"
                          "src/tasks/efc_task.h:Task"
                          "src/tasks/efc_taskspool.h:TasksPool"
                          "src/threads/efc_futex.h:Futex"
                          "src/threads/efc_mutex.h:Mutex"
                          "src/threads/efc_process.h:Process"
                          "src/threads/efc_thread.h:Thread"
                          "src/threads/efc_threads_common.h:threads_common.h"
                          "src/threads/efc_tlsvalue.h:TlsValue"
                          "src/threads/efc_waitcondition.h:WaitCondition"
                          "src/network/efc_byteorder.h:network/byteorder.h"
                          "src/network/efc_socket.h:network/Socket"
                          "src/network/efc_lasterror.h:network/LastError"
                          "src/network/efc_connection.h:network/Connection"
                          "src/network/efc_sender.h:network/Sender"
                          "src/network/efc_receiver.h:network/Receiver"
                          "src/network/efc_transport.h:network/Transport"
                          "src/network/efc_clientsocket.h:network/ClientSocket"
                          "src/network/efc_serversocket.h:network/ServerSocket"
                          "src/network/efc_asynctransport.h:network/AsyncTransport"
                          "src/network/efc_udpreceiver.h:network/UdpReceiver"
                          "src/network/efc_udpsender.h:network/UdpSender"
                          "src/network/efc_udpsocket.h:network/UdpSocket")
install_cmake_files ("cmake/FindEFC.cmake")
install_target (efc)
