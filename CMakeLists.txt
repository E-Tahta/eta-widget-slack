project(eta-widget-slack)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   )

set(slack_SRCS

    clock.cpp
    fileio.cpp
    networkmanager.cpp
    slack.cpp)


kde4_add_plugin(slack_library ${slack_SRCS})
target_link_libraries(slack_library
                      ${KDE4_KIO_LIBS}
                      ${KDE4_PLASMA_LIBS}
                      ${KDE4_KCMUTILS_LIBRARY}
                      ${KACTIVITIES_LIBRARY}
                      ${QT_QTDECLARATIVE_LIBRARY}
                    )



install(TARGETS slack_library DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/kde4/)
install(FILES eta-widget-slack.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/kde4/services/)
install(DIRECTORY package/ DESTINATION ${CMAKE_INSTALL_PREFIX}/share/kde4/apps/plasma/packages/tr.org.etap.slack)
