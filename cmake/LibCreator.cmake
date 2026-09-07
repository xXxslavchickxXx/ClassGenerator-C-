function(parse_and_delete_argument Array ParseARG OutVar)
    list(FIND ${Array} ${ParseARG} ARG_ITER)
    list(LENGTH ${Array} ARGN_SIZE)

    math(EXPR limit "${ARGN_SIZE}")
    math(EXPR arg_pos "${ARG_ITER} + 1")

    if (
        NOT ${ARG_ITER} EQUAL -1 AND
        ${arg_pos} LESS ${limit}
    )
        list(GET ${Array} ${arg_pos} ${OutVar})
        list(REMOVE_AT ${Array} ${arg_pos} ${ARG_ITER})
    else()
        set(${OutVar} "")
    endif()
    
    set(${OutVar} ${${OutVar}} PARENT_SCOPE)
    set(${Array} ${${Array}} PARENT_SCOPE)
endfunction()

function(is_access str flag)
    if (
        ${${str}} STREQUAL PRIVATE OR
        ${${str}} STREQUAL PUBLIC OR
        ${${str}} STREQUAL INTERFACE
    )
        set(${flag} TRUE PARENT_SCOPE)
    else()
        set(${flag} FALSE PARENT_SCOPE)
    endif()
endfunction()

function(is_in_list list item result)
    list(FIND ${list} ${item} INDEX)
    if (INDEX GREATER_EQUAL 0)
        set(${result} TRUE PARENT_SCOPE)
    else()
        set(${result} FALSE PARENT_SCOPE)
    endif()
endfunction()

function(set_state currentItem state)
    set(buissness_word FILE_SET BASE_DIRS FILES TYPE SOURCE_END)

    is_access(${currentItem} is_acs)
    if (is_acs)
        set(${state} NEW_SET PARENT_SCOPE)
        return()
    endif()

    is_in_list(buissness_word ${${currentItem}} is_bw)
    if (NOT is_bw)
        set(${state} "" PARENT_SCOPE)
        return()
    endif()

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
    endif()
endfunction()

# callback - функция, которая принимает один аргумент, а конкретно элемент списка
function(if_empty flag callback)
    foreach(item IN LISTS ARGN)
        if (NOT ${item})
            set(${flag} TRUE PARENT_SCOPE)
            if(callback)
                cmake_language(CALL ${callback} ${item})
            endif()
            return()
        endif()
    endforeach()
    set(${flag} FALSE PARENT_SCOPE)
endfunction()

# @brief Проверяет, является ли список ресуров интерфейсного формата
function(is_interfaceSource SourceArray flag)
    set(state COLLECTING_FILES)
    list(APPEND ${SourceArray} SOURCE_END)
    # state_list:
    # NEW_SET - состояние, когда обнаружено слово доступа, и надо начать вести новый список ресурсов
    # SET_TYPE - состояние выбора типа ресурса
    # SET_FILESET - состояние назначения имени ресурсной группы
    # COLLECTING_DIRS - состояние сбора базовых директорий ресурсной группы
    # COLLECTING_FILES - состояние сбора файлов ресурсной группы
    # END_STATE - состояние окончания исследования

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
                # Условие когда есть единицы трансляции, то есть ресурс не интерфейсный
                if(files AND NOT file_set)
                    set(${flag} FALSE PARENT_SCOPE)
                    return()
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
        elseif(state STREQUAL COLLECTING_DIRS)
            list(APPEND base_dirs ${item})
        elseif(state STREQUAL COLLECTING_FILES)
            list(APPEND files ${item})
        endif()
    endforeach()

    set(${flag} TRUE PARENT_SCOPE)
endfunction()

function(source_error var)
    message("[WARNING] The resource package is incomplete; the following is missing: ${var}")
endfunction()

function(target_source_setter custom_target SourceArray)
    set(state COLLECTING_FILES)
    list(APPEND ${SourceArray} SOURCE_END)
    # state_list:
    # NEW_SET - состояние, когда обнаружено слово доступа, и надо начать вести новый список ресурсов
    # SET_TYPE - состояние выбора типа ресурса
    # SET_FILESET - состояние назначения имени ресурсной группы
    # COLLECTING_DIRS - состояние сбора базовых директорий ресурсной группы
    # COLLECTING_FILES - состояние сбора файлов ресурсной группы
    # END_STATE - состояние окончания исследования

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
                    if_empty("" source_error base_dirs files)
                    if(type STREQUAL HEADERS)
                        target_sources(${custom_target}
                            
                        )
                    else()
                        if_empty("" source_error set_type)

                    endif()
                elseif(files)

                endif()

                # clear data for new set
                set(access ${item})
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
        elseif(state STREQUAL COLLECTING_DIRS)
            list(APPEND base_dirs ${item})
        elseif(state STREQUAL COLLECTING_FILES)
            list(APPEND files ${item})
        endif()
    endforeach()

    set(${flag} TRUE PARENT_SCOPE)
endfunction()

function(create_lib libName)
    set(ARG_LIST ${ARGN})

    parse_and_delete_argument(ARG_LIST LIB_TYPE LTYPE)

    # Первый проход. Проверяем, являются ли ресурсы интерфейсного типа
    is_interfaceSource(ARG_LIST flag)
    if (flag)
        add_library(${libName} INTERFACE)
        message("[STATUS] This source is interface")
    else()
        add_library(${libName} ${LTYPE})
        message("[STATUS] This source not a interface")
    endif()

    target_source_setter(${libName} ARG_LIST)

    set(${libName} ${${libName}} PARENT_SCOPE)
endfunction()