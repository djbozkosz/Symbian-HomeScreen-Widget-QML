## Description
<img src="https://s21.postimg.cc/gcakweiif/scr_95.jpg" alt="![Template dark - landscape" height="100" align="right">
Homescreen widget template for Symbian v5, Anna, Belle using Qt 4.7.4, QML, C++.

## Features
* used Qt QML

**Widget**
* encapsulated widget methods for simply use
* five widget types: _wide image_, _one row_, _tow rows_, _three rows_, _three text rows_
* possibility to pass QImage for image widget items
* dark / light / transparent widget skins included

**Application**
* possibility to run application in background with hidden thumbnail in task manager
* dark and light skin switch
* transparent statusbar and toolbar with blurred background
* reskined light statusbar
* addded statusbar title
* modified statusbar indicators layout

**Symbian Capability**  
Widget functions are built into shared library and for correct functionality must have application and widget library same capabilities. In bin folder are prepared libraries with following capabilities:

* NetworkServices
* NetworkServices, Location

For adding new capabilities please use SisContents application: Open sis installer package and locate widget dll library. Then edit the capabilities and extract modified dll into your project.

## Images
Widgets and application template:  
<img src="https://s21.postimg.cc/broehgyt3/scr_98.jpg" alt="Template widgets" width="200">
<img src="https://s21.postimg.cc/j53sgfiuv/scr_94.jpg" alt="![Template dark - portrait" width="200">
<img src="https://s21.postimg.cc/6dkkct2rb/scr_96.jpg" alt="![Template light - portrait" width="200">

<img src="https://s21.postimg.cc/gcakweiif/scr_95.jpg" alt="![Template dark - landscape" width="300">
<img src="https://s21.postimg.cc/o2cb4feif/scr_97.jpg" alt="![Template light - landscape" width="300">
