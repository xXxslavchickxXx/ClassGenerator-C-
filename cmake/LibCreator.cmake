# Скажу сразу: Легко сделать гибкую автоматизацию, то есть то что
# делает моя функция - невозможно!!! Мы либо делаем парсер, что
# само по себе капец как сложно, либо делаем колхозную дрянь
# в которой любой другой человек должен разбираться и запоминать что-то
# а тут и синтаксис таргет_срс, и работает все корректно
# добавить что-то новое, тоже легко, так что альтернатив простых
# нет, так что это далеко не переусложнение
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

# @details Почему конечный автомат?
# 
# Стандартный подход с if-ами не работает, потому что:
# 1. Список аргументов может содержать перемешанные PRIVATE/PUBLIC/FILE_SET
# 2. Нам нужно пропускать пустые группы ресурсов без ошибки
# 3. BASE_DIRS может идти как до, так и после FILES
# 4. HEADERS автоматически задает TYPE HEADERS без явного указания
# 5. Нужно отличать интерфейсную библиотеку (только заголовки) от обычной
# 
# Попытки упростить приводили к:
# - Дублированию кода
# - Невозможности обрабатывать краевые случаи
# - Ошибкам при пустых списках
# 
# Конечный автомат — единственное решение, которое покрывает все случаи
# и при этом остается расширяемым
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
                if (files AND NOT file_set)                    
                    if (NOT base_dirs OR file_set OR set_type)
                        set(${flag} FALSE PARENT_SCOPE)
                        return()
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

    set(${flag} TRUE PARENT_SCOPE)
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
                    if (NOT base_dirs OR files OR file_set OR set_type)
                        target_sources(${custom_target}
                            ${access}
                                FILE_SET ${file_set}
                                TYPE ${set_type}
                                BASE_DIRS ${base_dirs}
                                FILES ${files}
                        )
                    else()
                        message("[WARNING] The resource package is incomplete;")
                    endif()
                elseif(files)
                    if (NOT "${base_dirs} ${file_set} ${set_type}")
                        target_sources(${custom_target}
                            ${access}
                                ${files}
                        )
                    else()
                        message("[WARNING] Attention, translation units must not have header set attribute")
                    endif()
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
        elseif (state STREQUAL COLLECTING_DIRS)
            list(APPEND base_dirs ${item})
        elseif (state STREQUAL COLLECTING_FILES)
            list(APPEND files ${item})
        elseif (state)
            continue()
        endif()
    endforeach()

    set(${flag} TRUE PARENT_SCOPE)
endfunction()

# @brief Данная функция позволяет создавать библиотеку
# @details Да, существуют таргет_срс и просто эдд_либрари, которые
# делают абсолютно тоже самое, точнее на основе их и сделана эта
# функция, но за одним важным исключением, она позволяет не заботиться
# о ее сущности. Я имею ввиду, что когда вы ведете активную разработку
# довольно неприятной вещью является постоянное вмешательство в
# CMakeLists.txt эта функция призвана избавить вас от этого.
# То есть вы можете указывать еще не сформированные списки файлов
# и если их еще нет (они пусты), то функция вежливо пропустит их
# не кидая ошибку, как это делает таргет_срс. Также эта функция
# автоматически определяет тип библиотеки, что также избавляет от
# надобности этим заниматься, учитывая что это довольно бесполезное
# занятие, учитывая что существует всего 4 вида либ, тем не менее
# вы можете указать желаемый тип библиотеки, и в зависимости от ресурсов
# функция выберет соответствующий тип библиотеки.
function(create_lib libName)
    set(ARG_LIST ${ARGN})

    cmake_parse_arguments(arg "" "LIB_TYPE" "" ${ARGN})

    # Первый проход. Проверяем, являются ли ресурсы интерфейсного типа
    is_interfaceSource(ARG_LIST flag)
    if (flag)
        add_library(${libName} INTERFACE)
        message("[STATUS] This source is interface")
    else()
        add_library(${libName} ${arg_LIB_TYPE})
        message("[STATUS] This source not a interface")
    endif()

    target_source_setter(${libName} ARG_LIST)

    set(${libName} ${${libName}} PARENT_SCOPE)
endfunction()