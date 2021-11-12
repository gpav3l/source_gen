# README #

App for generation text by input data, and selected pattern. Main goal was create simple app, that help me generate verilog testbench and driver template for custom IP cores.
App build with Qt and MinGW compiler, and every generator make as plugin (yes, second aim was create test app for me that use plugin). 

### Installation ###

* Download repo and unzip it
* Open TestGen_proj.pro with QtCreator
* Add to release scheme custom build step use Makefile, and type in argument field "install" (without quots)
* Build release
* add to output folder Qt's dll (if Qt bin folder not in You system path)
* Use it
