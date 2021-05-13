TARGET ?= run
SRC_DIRS ?= . 

SRCS := $(shell find $(SRC_DIRS) -name '*.cc' -or -name '*.c' -or -name '*.s')
OBJS := $(addsuffix .o,$(basename $(SRCS)))
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d )
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -I /usr/include/gstreamer-1.0 -I /usr/include/glib-2.0 -I /usr/lib/glib-2.0/include -pthread -MMD -MP 

LDLIBS = -lglut -lGLU -lGL -lgstreamer-1.0 -lgobject-2.0 -lglib-2.0 

$(TARGET): $(OBJS)
	g++ $(LDFLAGS) $(OBJS) -o $@ $(LOADLIBES) $(LDLIBS)

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS)

-include $(DEPS)

