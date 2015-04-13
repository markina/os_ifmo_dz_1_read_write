all:
	cd lib && $(MAKE)
	cd cat && $(MAKE)
	cd revwords && $(MAKE)
	cd lenwords && $(MAKE)
	cd filter && $(MAKE)
	cd bufcat && $(MAKE)
	cd buffilter && $(MAKE)
rm:
	cd lib && $(MAKE) rm
	cd cat && $(MAKE) rm
	cd revwords && $(MAKE) rm
	cd lenwords && $(MAKE) rm
	cd filter && $(MAKE) rm
	cd bufcat && $(MAKE) rm
	cd buffilter && $(MAKE) rm