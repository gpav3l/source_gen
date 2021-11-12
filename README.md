# README #

App for generation text from input data and selected pattern. The main goal was created simple app, that help me generate Verilog testbench and driver template for custom IP cores.
App builds with Qt and MinGW compiler, and every generator make as a plugin (yes, second aim was create a test app for me that use plugin).

### Installation ###

* Download repo and unzip it
* Open TestGen_proj.pro with QtCreator
* Add to release scheme custom build step use Makefile, and type in argument field "install" (without quots)
* Build release
* add to output folder Qt's dll (if Qt bin folder not in You system path)
* Use it
