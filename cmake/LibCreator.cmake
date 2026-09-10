function(set_state currentItem state)
    if (${${currentItem}} STREQUAL FILE_SET)
        set(${state} SET_FILESET PARENT_SCOPE)
    elseif (${${currentItem}} STREQUAL BASE_DIRS)
        set(${state} COLLECTING_DIRS PARENT_SCOPE)
    elseif (${${currentItem}} STREQUAL FILES)
        set(${state} COLLECTING_FILES PARENT_SCOPE)
    elseif (${${currentItem}} STREQUAL TYPE)
        set(${state} SET_TYPE PARENT_SCOPE)
    elseif (${${currentItem}} STREQUAL SOURCE_END)
        set(${state} END_STATE PARENT_SCOPE)
    elseif (${${currentItem}} STREQUAL LIB_TYPE)
        set(${state} SKIP PARENT_SCOPE)
    else()
        if (
            ${${currentItem}} STREQUAL PRIVATE OR
            ${${currentItem}} STREQUAL PUBLIC OR
            ${${currentItem}} STREQUAL INTERFACE
        )
            set(${state} NEW_SET PARENT_SCOPE)
        else()
            set(${state} "" PARENT_SCOPE)
        endif()
    endif()
endfunction()

# @brief Функция для парсинга ресурсных групп
#
# @details Функция создает переменны с префиксом в виде имения самой группы,
# то есть, например есть группа HEADERS, то создатся 5 переменных с префиксом
# HEADERS:
#   HEADERS_file_set
#   HEADERS_type
#   HEADERS_access
#   HEADERS_files
#   HEADERS_base_dirs
#
# Чтобы внешний вызов знал, какие группы переменных созданы,
# в аргументы передается переменная, которая сохраняет все
# именна группы, для внешней итерации по переменных
#
# Также важно отметить то, что группы без имени, то есть без FILE_SET
# являются единицами траснляции, и обозначаются именем: SOURCES
#
# @arg SourceArray - аргумент с сырым списком ресурсов,
# который будет парсить функция
# @arg GroupsArray - возвращаемый аргумент, в котором
# содержатся имена всех ресурсных групп
function(parse_blocks SourceArray GroupsArray)
    set(state COLLECTING_FILES)
    list(APPEND ${SourceArray} SOURCE_END)
    # state_list:
    # NEW_SET - состояние, когда обнаружено слово доступа, и надо начать вести новый список ресурсов
    # SET_TYPE - состояние выбора типа ресурса
    # SET_FILESET - состояние назначения имени ресурсной группы
    # COLLECTING_DIRS - состояние сбора базовых директорий ресурсной группы
    # COLLECTING_FILES - состояние сбора файлов ресурсной группы
    # END_STATE - состояние окончания исследования
    set(groups "")

    set(access PRIVATE)
    set(file_set "")
    set(set_type "")
    set(base_dirs "")
    set(files "")
    foreach(item IN LISTS ${SourceArray})
        set_state(item candidate_state)

        # Если меняется состояние всегда делаем переход к следующей итерации.
        # Кроме ситуации, когда задается новая группа ресурсов
        if (candidate_state)
            if (
                candidate_state STREQUAL NEW_SET OR
                candidate_state STREQUAL END_STATE
            )
                if (file_set)
                    if (base_dirs AND files AND file_set AND set_type)
                        list(FIND groups ${file_set} exist)
                        if (exist STREQUAL -1)
                            list(APPEND groups ${file_set})
                        endif()

                        # Создаем переменные с ресурсной группой
                        set(${file_set}_access ${access} PARENT_SCOPE)
                        set(${file_set}_files ${files} PARENT_SCOPE)
                        set(${file_set}_base_dirs ${base_dirs} PARENT_SCOPE)
                        set(${file_set}_file_set ${file_set} PARENT_SCOPE)
                        set(${file_set}_type ${set_type} PARENT_SCOPE)
                    else()
                        message("[WARNING] The resource package is empty;")
                    endif()
                elseif(files)
                    if (NOT base_dirs AND NOT file_set AND NOT set_type)
                        list(FIND groups SOURCES exist)
                        if (exist STREQUAL -1)
                            list(APPEND groups SOURCES)
                        endif()

                        # Создаем переменные с группой единиц трансляции
                        set(SOURCES_access ${access} PARENT_SCOPE)
                        set(SOURCES_files ${files} PARENT_SCOPE)
                    else()
                        message("[WARNING] Attention, translation units must not have header set attribute")
                    endif()
                endif()
                # target_source in the second pass
                set(access ${item})

                # clear set_data
                set(state COLLECTING_FILES)
                set(file_set "")
                set(set_type "")
                set(base_dirs "")
                set(files "")
            else()
                set(state ${candidate_state})
            endif()
            continue()
        endif()

        if(state STREQUAL SET_FILESET)
            set(file_set ${item})
            if(${item} STREQUAL HEADERS)
                set(set_type ${item})
            endif()
        elseif (state STREQUAL SET_TYPE)
            set(set_type ${item})
        elseif (state STREQUAL COLLECTING_DIRS)
            list(APPEND base_dirs ${item})
        elseif (state STREQUAL COLLECTING_FILES)
            list(APPEND files ${item})
        elseif (state)
            continue()
        endif()
    endforeach()

    set(${GroupsArray} ${groups} PARENT_SCOPE)
endfunction()

function(create_lib libName)
    set(ARG_LIST ${ARGN})

    cmake_parse_arguments(arg "" "LIB_TYPE" "" ${ARGN})

    parse_blocks(ARG_LIST groups)
    
    # Первый проход. Проверяем, если нет группы единиц трансляции,
    # то это интерфейсная либа
    if (NOT SOURCES_files)
        set(is_interface_lib TRUE)
    endif()

    # Создаем либу, если ресурсов нет или ресурсы только хедерные
    if (is_interface_lib)
        add_library(${libName} INTERFACE)
        message("[STATUS] This source is interface")
    else()
        add_library(${libName} ${arg_LIB_TYPE})
        message("[STATUS] This source not a interface")
    endif()
    
    foreach(group IN LISTS groups)
        if(group STREQUAL SOURCES AND ${group}_files)
            if (NOT ${group}_base_dirs OR ${group}_file_set OR ${group}_type)
                target_sources(${libName}
                    ${${group}_access}
                        ${${group}_files}
                )
            endif()
        elseif (${group}_file_set)
            if (${group}_base_dirs AND ${group}_files AND ${group}_file_set AND ${group}_type)
                target_sources(${libName}
                    ${${group}_access}
                        FILE_SET ${${group}_file_set}
                        TYPE ${${group}_type}
                        BASE_DIRS ${${group}_base_dirs}
                        FILES ${${group}_files}
                )
            endif()
        endif()
    endforeach()

    set(${libName} ${${libName}} PARENT_SCOPE)
endfunction()