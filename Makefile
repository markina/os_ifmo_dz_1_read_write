all:
	cd lib && $(MAKE)
	cd cat && $(MAKE)
	cd revwords && $(MAKE)
	cd lenwords && $(MAKE)
rm:
	cd lib && $(MAKE) rm
	cd cat && $(MAKE) rm
	cd revwords && $(MAKE) rm
	cd lenwords && $(MAKE) rm