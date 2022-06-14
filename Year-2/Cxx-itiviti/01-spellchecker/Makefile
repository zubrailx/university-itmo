CXX = clang++
CXXFLAGS = -std=c++17
CXXFLAGS.DEBUG = $(CXXFLAGS) -Wall -Werror -g
CXXFLAGS.SANITIZED = $(CXXFLAGS.DEBUG) -fsanitize=address,undefined,leak
CXXFLAGS.RELEASE = $(CXXFLAGS.DEBUG) -O3

LD = $(CXX)
LDFLAGS = 
LDFLAGS.DEBUG = $(LDFLAGS) 
LDFLAGS.SANITIZED = $(LDFLAGS) -fsanitize=address,undefined,leak
LDFLAGS.RELEASE = $(LDFLAGS)

TARGET = spellchecker

SRCDIR = src
BUILDDIR = build
BUILDDIR.DEBUG = $(BUILDDIR)/debug
BUILDDIR.SANITIZED = $(BUILDDIR)/sanitized
BUILDDIR.RELEASE = $(BUILDDIR)/release

SOURCES = $(shell find $(SRCDIR) -type f -name "*.cpp")
HEADERS = $(shell find $(SRCDIR) -type f -name "*.h")
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, %.o, $(SOURCES))


$(BUILDDIR.DEBUG)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS.DEBUG) -c $< -o $@

$(BUILDDIR.SANITIZED)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS.SANITIZED) -c $< -o $@

$(BUILDDIR.RELEASE)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS.RELEASE) -c $< -o $@


.PHONY: all

all: debug debug.run
# -----------------------------------------------------------------------------
.PHONY += debug debug.run
OBJECTS.DEBUG = $(patsubst %, $(BUILDDIR.DEBUG)/%, $(OBJECTS))
debug: DIRS = $(dir $(OBJECTS.DEBUG))
debug: mkdir $(OBJECTS.DEBUG)
	$(LD) $(LDFLAGS.DEBUG) $(OBJECTS.DEBUG) -o $(BUILDDIR.DEBUG)/$(TARGET)

debug.run:
	@./$(BUILDDIR.DEBUG)/$(TARGET)

# -----------------------------------------------------------------------------
.PHONY += sanitized sanitized.run
OBJECTS.SANITIZED = $(patsubst %, $(BUILDDIR.SANITIZED)/%, $(OBJECTS))
sanitized: DIRS = $(dir $(OBJECTS.SANITIZED))
sanitized: mkdir $(OBJECTS.SANITIZED)
	$(LD) $(LDFLAGS.SANITIZED) $(OBJECTS.SANITIZED) -o $(BUILDDIR.SANITIZED)/$(TARGET)

sanitized.run: 
	@./$(BUILDDIR.SANITIZED)/$(TARGET)

# -----------------------------------------------------------------------------
.PHONY += release release.run
OBJECTS.RELEASE = $(patsubst %, $(BUILDDIR.RELEASE)/%, $(OBJECTS))
release: DIRS = $(dir $(OBJECTS.RELEASE))
release: mkdir $(OBJECTS.RELEASE)
	$(LD) $(LDFLAGS.RELEASE) $(OBJECTS.RELEASE) -o $(BUILDDIR.RELEASE)/$(TARGET)

release.run:
	@./$(BUILDDIR.RELEASE)/$(TARGET)

# -----------------------------------------------------------------------------
.PHONY += mkdir clean
mkdir:
	mkdir -p $(DIRS)

clean:
	rm -rf $(BUILDDIR)

