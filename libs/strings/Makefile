include ../../dpf/Makefile.base.mk

STRINGS_LIB = ../../build/strings/libstrings.a

STRINGS_SOURCES = \
	ResonantArrayAVX.cpp \
	ResonantArray.cpp \
	ResonantArraySSE.cpp \
	ResonantStringAVX.cpp \
	ResonantString.cpp \
	ResonantStringSSE.cpp

STRINGS_OBJS = $(patsubst %.cpp,../../build/strings/%.o,$(STRINGS_SOURCES))

ifeq ($(CPU_I386_OR_X86_64),true)
AVX_FLAGS = -mavx
SSE_FLAGS = -msse
endif

all: $(STRINGS_LIB)

clean:
	rm -f $(STRINGS_LIB) $(STRINGS_OBJS)

$(STRINGS_LIB): $(STRINGS_OBJS)
	@mkdir -p $(dir $@)
	@echo "Creating $@"
	$(SILENT)$(AR) crs $@ $^

../../build/strings/%AVX.o: %AVX.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	$(SILENT)$(CXX) $< $(BUILD_CXX_FLAGS) $(AVX_FLAGS) -c -o $@

../../build/strings/%SSE.o: %SSE.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	$(SILENT)$(CXX) $< $(BUILD_CXX_FLAGS) $(SSE_FLAGS) -c -o $@

../../build/strings/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	$(SILENT)$(CXX) $< $(BUILD_CXX_FLAGS) -c -o $@

.PHONY: all clean

-include $(STRINGS_OBJS:%.o=%.d)
