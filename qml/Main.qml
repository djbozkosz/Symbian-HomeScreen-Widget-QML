//------------------------------------------------------------------------------
import QtQuick 1.1;
import com.nokia.symbian 1.1;

//------------------------------------------------------------------------------
Window {
  id: main;
  platformInverted: false;

  property bool platformSymbian: false;

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
      height: parent.height;
      verticalAlignment: Text.AlignVCenter;
      //platformInverted: main.platformInverted;
      font {
        family: platformStyle.fontFamilyRegular;
        pixelSize: platformStyle.fontSizeSmall + platformStyle.paddingSmall * 0.5;
      }
      anchors {
        left: parent.left;
        leftMargin: platformStyle.paddingSmall;
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

  function initPage()
  {
    mainPageStack.replace(Qt.resolvedUrl("Page123.qml"));
  }

  function showMainToolBarMenu()
  {
    mainToolBarMenu.open();
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

  function setBackground(bg, bgBlur)
  {
    mainBackground.source = bg;
    mainBackgroundStatus.source = bgBlur;
    mainBackgroundToolBar.source = bgBlur;
  }

  function setPlatformSymbian(platform)
  {
    platformSymbian = platform;
  }

  function onResize()
  {
    mainPageStack.currentPage.onResize();
  }
}
//------------------------------------------------------------------------------
