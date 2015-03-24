CC := g++
SRCDIR := src
BUILDDIR := build
TARGET := bin/μ-colander
OUTDIR := out

OUT := $(shell find $(OUTDIR) -type f -name *.png)

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))

OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall -std=c++11 

LIB := -lopencv_core -lopencv_highgui -lopencv_videoio -lopencv_imgproc -lopencv_video -lopencv_imgcodecs
INC := -I /usr/local/include 
#$(pkg-config --cflags opencv) 

$(TARGET) : $(OBJECTS)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning..."
	@echo "$(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)
	@echo "$(RM) $(OUT)"; $(RM) $(OUT)

test : $(TARGET)
	./$(TARGET) test/40x-coastal-3rdchannel_1033-1221.avi

.PHONY: clean test

