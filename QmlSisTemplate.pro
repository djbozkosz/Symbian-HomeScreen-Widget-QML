# QML
QT += declarative #network
CONFIG += qt-components

# netowrk
symbian:TARGET.CAPABILITY += NetworkServices

# mobility
#CONFIG += mobility
#MOBILITY +=

TARGETNAME = QmlSisTemplate
symbian:DEPLOYMENT.display_name = Qml Sis Template
TARGETUID3 = 0xAC928700
symbian:VERSION = 0.0.1
ICONPATHNAME = images/icons/icon.svg

symbian:DEFINES += TARGETUID3=\"$${TARGETUID3}\"
!symbian:DEFINES += TARGETUID3=\\\"$${TARGETUID3}\\\"
symbian:TARGET.UID3 = $${TARGETUID3}

QMAKE_CXXFLAGS += -pedantic
QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS += -Wno-invalid-offsetof
QMAKE_CXXFLAGS += -Wno-psabi
QMAKE_CXXFLAGS += -fno-strict-aliasing
QMAKE_CXXFLAGS += -U__STRICT_ANSI__
QMAKE_CXXFLAGS += -O3

SOURCES += \
    src/main.cpp \
    src/mainQml.cpp

HEADERS += \
    src/mainQml.h \
    src/homeScreenWidget.h

FORMS +=

RESOURCES += \
    qrc/qresources.qrc

OTHER_FILES +=

#homescreen widget
symbian:LIBS += -lapgrfx -lcone -lws32 -lbitgdi -lfbscli -laknskins -laknskinsrv -leikcore -lavkon
HSWIDGETLIB = HSWidgetPlugin.dll
symbian:DEFINES += HSWIDGETLIB=\"$${TARGETNAME}$${HSWIDGETLIB}\"
!symbian:DEFINES += HSWIDGETLIB=\\\"$${TARGETNAME}$${HSWIDGETLIB}\\\"
symbian:deploymentFiles.pkg_postrules += "\"bin/"$${HSWIDGETLIB}"\" - \"!:/sys/bin/"$${TARGETNAME}$${HSWIDGETLIB}"\""
symbian:DEPLOYMENT += deploymentFiles

#deployment
deploymentFolder.source = qml
deploymentFolder.target = .
DEPLOYMENTFOLDERS = deploymentFolder

defineTest(qtcAddDeployment) {
    for(deploymentfolder, DEPLOYMENTFOLDERS) {
        item = item$${deploymentfolder}
        itemsources = $${item}.sources
        $$itemsources = $$eval($${deploymentfolder}.source)
        itempath = $${item}.path
        $$itempath= $$eval($${deploymentfolder}.target)
        export($$itemsources)
        export($$itempath)
        DEPLOYMENT += $$item
    }

    MAINPROFILEPWD = $$PWD

    symbian {
        isEmpty(ICON):exists($${ICONPATHNAME}):ICON = $${ICONPATHNAME}
        isEmpty(TARGET.EPOCHEAPSIZE):TARGET.EPOCHEAPSIZE = 0x20000 0x10000000
    } else:win32 {
        copyCommand =
        for(deploymentfolder, DEPLOYMENTFOLDERS) {
            source = $$MAINPROFILEPWD/$$eval($${deploymentfolder}.source)
            source = $$replace(source, /, \\)
            sourcePathSegments = $$split(source, \\)
            target = $$OUT_PWD/$$eval($${deploymentfolder}.target)/$$last(sourcePathSegments)
            target = $$replace(target, /, \\)
            target ~= s,\\\\\\.?\\\\,\\,
            !isEqual(source,$$target) {
                !isEmpty(copyCommand):copyCommand += &&
                isEqual(QMAKE_DIR_SEP, \\) {
                    copyCommand += $(COPY_DIR) \"$$source\" \"$$target\"
                } else {
                    source = $$replace(source, \\\\, /)
                    target = $$OUT_PWD/$$eval($${deploymentfolder}.target)
                    target = $$replace(target, \\\\, /)
                    copyCommand += test -d \"$$target\" || mkdir -p \"$$target\" && cp -r \"$$source\" \"$$target\"
                }
            }
        }
        !isEmpty(copyCommand) {
            copyCommand = @echo Copying application data... && $$copyCommand
            copydeploymentfolders.commands = $$copyCommand
            first.depends = $(first) copydeploymentfolders
            export(first.depends)
            export(copydeploymentfolders.commands)
            QMAKE_EXTRA_TARGETS += first copydeploymentfolders
        }
    } else:unix {
        maemo5 {
            desktopfile.files = $${TARGET}.desktop
            desktopfile.path = /usr/share/applications/hildon
            icon.files = $${TARGET}64.png
            icon.path = /usr/share/icons/hicolor/64x64/apps
        } else:!isEmpty(MEEGO_VERSION_MAJOR) {
            desktopfile.files = $${TARGET}_harmattan.desktop
            desktopfile.path = /usr/share/applications
            icon.files = $${TARGET}80.png
            icon.path = /usr/share/icons/hicolor/80x80/apps
        } else {
            copyCommand =
            for(deploymentfolder, DEPLOYMENTFOLDERS) {
                source = $$MAINPROFILEPWD/$$eval($${deploymentfolder}.source)
                source = $$replace(source, \\\\, /)
                macx {
                    target = $$OUT_PWD/$${TARGET}.app/Contents/Resources/$$eval($${deploymentfolder}.target)
                } else {
                    target = $$OUT_PWD/$$eval($${deploymentfolder}.target)
                }
                target = $$replace(target, \\\\, /)
                sourcePathSegments = $$split(source, /)
                targetFullPath = $$target/$$last(sourcePathSegments)
                targetFullPath ~= s,/\\.?/,/,
                !isEqual(source,$$targetFullPath) {
                    !isEmpty(copyCommand):copyCommand += &&
                    copyCommand += $(MKDIR) \"$$target\"
                    copyCommand += && $(COPY_DIR) \"$$source\" \"$$target\"
                }
            }
            !isEmpty(copyCommand) {
                copyCommand = @echo Copying application data... && $$copyCommand
                copydeploymentfolders.commands = $$copyCommand
                first.depends = $(first) copydeploymentfolders
                export(first.depends)
                export(copydeploymentfolders.commands)
                QMAKE_EXTRA_TARGETS += first copydeploymentfolders
            }
        }
        installPrefix = /opt/$${TARGET}
        for(deploymentfolder, DEPLOYMENTFOLDERS) {
            item = item$${deploymentfolder}
            itemfiles = $${item}.files
            $$itemfiles = $$eval($${deploymentfolder}.source)
            itempath = $${item}.path
            $$itempath = $${installPrefix}/$$eval($${deploymentfolder}.target)
            export($$itemfiles)
            export($$itempath)
            INSTALLS += $$item
        }

        !isEmpty(desktopfile.path) {
            export(icon.files)
            export(icon.path)
            export(desktopfile.files)
            export(desktopfile.path)
            INSTALLS += icon desktopfile
        }

        target.path = $${installPrefix}/bin
        export(target.path)
        INSTALLS += target
    }

    export (ICON)
    export (INSTALLS)
    export (DEPLOYMENT)
    export (TARGET.EPOCHEAPSIZE)
    export (TARGET.CAPABILITY)
    export (LIBS)
    export (QMAKE_EXTRA_TARGETS)
}

qtcAddDeployment()
