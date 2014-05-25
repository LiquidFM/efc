project (efc)

# Project header
project_cxx_library_header_default ()

# 3rdparty
list (APPEND ${PROJECT_NAME}_LIBS platform brolly)

# Sources
add_subdirectory (src)

# Target
add_library (efc STATIC ${${PROJECT_NAME}_SOURCES})
target_link_libraries (efc ${${PROJECT_NAME}_LIBS})

# Version
build_version_file (efc 0.0.1)

# Documentation
add_documentation ("efc" 0.0.1 "Exceptions Free Containers library")

# Install rules
install_header_files (efc "src/efc_list.h:List"
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
                          "src/threads/efc_waitcondition.h:WaitCondition")
install_target (efc)
