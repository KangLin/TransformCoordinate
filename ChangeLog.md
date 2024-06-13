- v1.1.0
  - Modify TransformCoordinateFiles

- v1.0.4
  - Modify prompt

- v1.0.3
  - Modify org.Rabbit.TransformCoordinate.desktop
  - Rename icon theme rabbit-*
  - Modify version format

- v1.0.2
  - debian: Check whether the RabbitCommon development library
    is installed on the system, and if so, install it on the system.
    If not, compile RabbitCommon from source
    and install it to /opt/TransformCoordinate
  - Rename share/TransformCoordinate.desktop to share/org.Rabbit.TransformCoordinate.desktop

- v1.0.1
  - Modify documents
  - Modify ci

- v1.0.0
  - Use [RabbitCommon v2.2.1](https://github.com/KangLin/RabbitCommon/releases/tag/v2.2.1)

- v0.0.11
  + Fix android bug
  + Replace RabbitCommon::CDir::GetOpenDirectory with QFileDialog::getExistingDirectory

- v0.0.10
  + Modify ci
    + Add github actions
  + Use RabbitCommon V2
    + Add Log
    + Add set style
  + Use CPack to package

- v0.0.9
  + Add android sign
  + Modify update

- v0.0.8
  + Modify translation
  + Modify ci
  + Add install runtime

- v0.0.7
  + Modify translation
  + Modify open file
  + Modify status bar prompt
  + Modify CMAKE_INSTALL_PREFIX for android
  + Modify translation and README
  + android hide splash screen

- v0.0.6
  + Transform coordinate
  + Modify translation
