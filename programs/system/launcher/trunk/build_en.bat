@erase lang.inc
@echo lang fix en >lang.inc
@fasm launcher.asm launcher
@kpack launcher
@erase lang.inc
@pause