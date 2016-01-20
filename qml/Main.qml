//------------------------------------------------------------------------------
import QtQuick 1.1;
import com.nokia.symbian 1.1;

//------------------------------------------------------------------------------
Window {
  id: main;
  platformInverted: false;

  property bool platformSymbian: false;
  property string bg;
  property string bgInverse;
  property string bgBlur;
  property string bgBlurInverse;

  Image {
    id: mainBackground;
    fillMode: Image.PreserveAspectCrop;
    smooth: true;
    anchors {
      fill: parent;
    }
  }

  Item {
    id: mainBackgroundStatusMask;
    clip: true;
    anchors {
      fill: mainStatusBar;
    }

    Image {
      id: mainBackgroundStatus;
      fillMode: Image.PreserveAspectCrop;
      smooth: true;
      anchors {
        fill: parent;
        bottomMargin: - mainBackground.height + mainBackgroundStatusMask.height;
      }
    }
  }

  Item {
    id: mainBackgroundToolBarMask;
    clip: true;
    anchors {
      fill: mainToolBar2;
    }

    Image {
      id: mainBackgroundToolBar;
      fillMode: Image.PreserveAspectCrop;
      smooth: true;
      anchors {
        fill: parent;
        topMargin: - mainBackground.height + mainBackgroundToolBarMask.height;
      }
    }
  }

  PageStack {
    id: mainPageStack;
    initialPage: mainPage;
    anchors {
      fill: parent;
    }
  }

  Page {
    id: mainPage;
    tools: mainToolBar;

    Item {
      id: mainContent;
      anchors {
        fill: parent;
        topMargin: mainStatusBar.height;
        bottomMargin: parent.tools.height;
      }
    }

    ToolBar {
      id: mainToolBar;
      platformInverted: main.platformInverted;
      anchors {
        bottom: parent.bottom;
      }

      tools: ToolBarLayout {
      }
    }

    function onResize() {}
  }

  StatusBar {
    id: mainStatusBar;
    platformInverted: main.platformInverted;
    anchors {
      top: parent.top;
    }

    Label {
      id: mainStatusBarTitle;
      text: "Qml Sis Template";
      verticalAlignment: Text.AlignVCenter;
      clip: true;
      platformInverted: main.platformInverted;
      font {
        pixelSize: platformStyle.fontSizeSmall + platformStyle.paddingSmall * 0.2;
      }
      anchors {
        fill: parent;
        leftMargin: platformStyle.paddingSmall;
        rightMargin: 190;
      }
    }
  }

  ToolBarLayout {
    id: mainToolBar2;
    visible: false;
    anchors {
      bottom: parent.bottom;
    }
  }

  Menu {
    id: mainToolBarMenu;
    platformInverted: main.platformInverted;

    content: MenuLayout {
      MenuItem {
        id: mainToolBarMenuExit;
        text: "Exit";
        platformInverted: main.platformInverted;
        onClicked: qmlView.quit(true);
      }
    }
  }

//------------------------------------------------------------------------------
  onOrientationChangeAboutToStart: onResize();
  onPlatformInvertedChanged: onPlatformInverted();

  function initPage()
  {
    mainPageStack.replace(Qt.resolvedUrl("Page123.qml"));
    onPlatformInverted();
  }

  function pageBack()
  {
    if(mainPageStack.depth <= 1)
    {
      qmlView.quit(false);
      return;
    }

    mainPageStack.pop();
  }

  function onResize()
  {
    mainPageStack.currentPage.onResize();
  }

  function onPlatformInverted()
  {
    var statusBg = platformInverted ? "qrc:/backgrounds/images/backgrounds/statusbar_inverse.png" : "qrc:/backgrounds/images/backgrounds/statusbar.png";
    var statusTextColor = platformInverted ? platformStyle.colorNormalLightInverted : platformStyle.colorNormalLight;
    var statusSignalLevel = platformInverted ? "qrc:/icons/images/icons/signal_level_bg_inverse.png" : "qrc:/icons/images/icons/signal_level_bg.png";
    var statusSignalLevelFull = platformInverted ? "qrc:/icons/images/icons/signal_level_full_inverse.png" : "qrc:/icons/images/icons/signal_level_full.png";
    var statusBatteryLevel = platformInverted ? (privateBatteryInfo.powerSaveModeEnabled ? "qrc:/icons/images/icons/battery_level_psm_bg_inverse.png" : "qrc:/icons/images/icons/battery_level_bg_inverse.png") : (privateBatteryInfo.powerSaveModeEnabled ? "qrc:/icons/images/icons/battery_level_psm_bg.png" : "qrc:/icons/images/icons/battery_level_bg.png");
    var statusBatteryLevelFull = platformInverted ? (privateBatteryInfo.powerSaveModeEnabled ? "qrc:/icons/images/icons/battery_level_psm_full_inverse.png" : "qrc:/icons/images/icons/battery_level_full_inverse.png") : (privateBatteryInfo.powerSaveModeEnabled ? "qrc:/icons/images/icons/battery_level_psm_full.png" : "qrc:/icons/images/icons/battery_level_full.png");
    var statusBatteryPSM = platformInverted ? "qrc:/icons/images/icons/battery_psm_inverse.png" : "qrc:/icons/images/icons/battery_psm.png";

    var toolBarBg = platformInverted ? "qrc:/backgrounds/images/backgrounds/toolbar_inverse.png" : "qrc:/backgrounds/images/backgrounds/toolbar.png";
    var sb = platformSymbian ? mainStatusBar.children[0].children[0].children[0] : mainStatusBar.children[1];

    sb.source = statusBg;
    sb.children[0].indicatorColor = statusTextColor;
    sb.children[1].color = statusTextColor;
    sb.children[2].source = statusSignalLevel;
    sb.children[2].children[0].children[0].source = statusSignalLevelFull;
    sb.children[3].source = statusBatteryLevel;
    sb.children[3].anchors.rightMargin = platformStyle.paddingLarge;
    sb.children[3].children[0].children[0].source = statusBatteryLevelFull;
    sb.children[3].children[1].source = statusBatteryPSM;
    sb.children[4].color = statusTextColor;

    mainToolBar.children[0].source = toolBarBg;

    if(mainPageStack.currentPage)
      mainPageStack.currentPage.tools.children[0].source = mainToolBar.children[0].source;

    updateBackground();
  }
//------------------------------------------------------------------------------
  function showMainToolBarMenu()
  {
    mainToolBarMenu.open();
  }
//------------------------------------------------------------------------------
  function setBackground(bg2, bgInverse2, bgBlur2, bgBlurInverse2)
  {
    bg = bg2;
    bgInverse = bgInverse2;
    bgBlur = bgBlur2;
    bgBlurInverse = bgBlurInverse2;

    updateBackground();
  }

  function updateBackground()
  {
    if(platformInverted)
    {
      mainBackground.source = bgInverse;
      mainBackgroundStatus.source = bgBlurInverse;
      mainBackgroundToolBar.source = bgBlurInverse;
    }
    else
    {
      mainBackground.source = bg;
      mainBackgroundStatus.source = bgBlur;
      mainBackgroundToolBar.source = bgBlur;
    }
  }

  function setPlatformSymbian(platform)
  {
    platformSymbian = platform;
  }
}
//------------------------------------------------------------------------------
