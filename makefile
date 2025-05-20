SRC_DIR := src
TST_DIR := unit
OBJ_DIR := obj
BIN_DIR := bin

EXE := $(BIN_DIR)/GL
CSRC := $(wildcard $(SRC_DIR)/*.c)
XSRC := $(wildcard $(SRC_DIR)/*.cpp)
COBJ := $(CSRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
XOBJ := $(XSRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

PLEXE := $(BIN_DIR)/utPlanet
PLXOBJ := $(OBJ_DIR)/utPlanet.o  $(OBJ_DIR)/planet.o
XMEXE := $(BIN_DIR)/utXML
XMXOBJ := $(OBJ_DIR)/utXML.o $(OBJ_DIR)/xml.o $(OBJ_DIR)/planet.o

CXXFLAGS := -Iinclude -Idep
LDLIBS = -lglfw3 -ldl -lpthread

.phony: all clean fresh release test

release: $(EXE)

test: $(PLEXE) $(XMEXE)

fresh: clean all

all: $(EXE) $(TEXE)

clean:
	@$(RM) -r $(BIN_DIR) $(OBJ_DIR)

$(EXE): $(COBJ) $(XOBJ) | $(BIN_DIR)
	@$(CXX) $^ $(LDLIBS) -g -pg -o $@

$(PLEXE): $(PLXOBJ) | $(BIN_DIR)
	@$(CXX) $^ $(LDLIBS) -g -pg -o $@

$(XMEXE): $(XMXOBJ) | $(BIN_DIR)
	@$(CXX) $^ $(LDLIBS) -g -pg -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c* | $(OBJ_DIR)
	@$(CXX) $(CXXFLAGS) -g -pg -c $< -o $@

$(OBJ_DIR)/%.o: $(TST_DIR)/%.c* | $(OBJ_DIR)
	@$(CXX) $(CXXFLAGS) -g -pg -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	@mkdir -p $@

makefile: ;

-include $(OBJ:.o=.d)
