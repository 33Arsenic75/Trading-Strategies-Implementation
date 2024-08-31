CXX = g++
CXXFLAGS = -w -std=c++11
TARGET = program
# .SILENT:
# Default rule to build the target
all: clean $(TARGET) run

# Rule to compile source files and generate the executable
$(TARGET): Database/Database.cpp
ifeq ($(strategy), BASIC)
	$(CXX) $(CXXFLAGS) Database/Database.cpp Basic/Basic.cpp -o $(TARGET)
else ifeq ($(strategy), DMA)
	$(CXX) $(CXXFLAGS) Database/Database.cpp Dma/Dma.cpp -o $(TARGET)
else ifeq ($(strategy), DMA++)
	$(CXX) $(CXXFLAGS) Database/Database.cpp Improved_Dma/Improved_dma.cpp -o $(TARGET)
else ifeq ($(strategy), MACD)
	$(CXX) $(CXXFLAGS) Database/Database.cpp Macd/macd.cpp -o $(TARGET)
else ifeq ($(strategy), RSI)
	$(CXX) $(CXXFLAGS) Database/Database.cpp Rsi/Rsi.cpp -o $(TARGET)
else ifeq ($(strategy), ADX)
	$(CXX) $(CXXFLAGS) Database/Database.cpp Adx/Adx.cpp -o $(TARGET)
else ifeq ($(strategy), LINEAR_REGRESSION)
	$(CXX) $(CXXFLAGS) Database/Database.cpp LinearModel/LinearModel.cpp LinearModel/Linear_Regression.cpp -o $(TARGET)
else ifeq ($(strategy), BEST_OF_ALL)
	$(CXX) $(CXXFLAGS) Best/Best.cpp Best/Database.cpp Best/Basic.cpp Best/Dma.cpp Best/improved_dma.cpp Best/Adx.cpp Best/Rsi.cpp Best/LinearModel.cpp Best/Linear_Regression.cpp  -o $(TARGET)
# else ifeq ($(strategy), PAIRS)
# 	$(CXX) $(CXXFLAGS) Database/Database.cpp Mean_reverting/mean_reverting.cpp -o $(TARGET)
else ifeq ($(strategy), PAIRS)
	$(CXX) $(CXXFLAGS) mean_reverting.cpp -o $(TARGET)
endif


# Rule to run the program
run:
ifeq ($(strategy), BASIC)
	./$(TARGET) $(symbol) $(n) $(x) $(start_date) $(end_date)
else ifeq ($(strategy), DMA)
	./$(TARGET) $(symbol) $(n) $(x) $(p) $(start_date) $(end_date)
else ifeq ($(strategy), DMA++)
	./$(TARGET) $(symbol) $(n) $(x) $(p) $(max_hold_days) $(c1) $(c2) $(start_date) $(end_date)
else ifeq ($(strategy), MACD)
	./$(TARGET) $(symbol) $(x) $(start_date) $(end_date)
else ifeq ($(strategy), RSI)
	./$(TARGET) $(symbol) $(n) $(x) $(oversold_threshold) $(overbought_threshold) $(start_date) $(end_date)
else ifeq ($(strategy), ADX)
	./$(TARGET) $(symbol) $(n) $(x) $(adx_threshold) $(start_date) $(end_date)
else ifeq ($(strategy), LINEAR_REGRESSION)
	./$(TARGET) $(symbol) $(x) $(p) $(train_start_date) $(train_end_date) $(start_date) $(end_date)
else ifeq ($(strategy), BEST_OF_ALL)
	./$(TARGET) $(symbol) $(start_date) $(end_date)
# else ifeq ($(strategy), PAIRS)
# 	./$(TARGET) $(symbol1) $(symbol2) $(x) $(n) $(threshold) $(start_date) $(end_date)
else ifeq ($(strategy), PAIRS)
	./$(TARGET) $(symbol1) $(symbol2) $(x) $(n) $(threshold) $(stop_loss_threshold)  $(start_date) $(end_date) 
endif

# Clean rule to remove the executable
clean:
	rm -f $(TARGET)

# Phony targets
.PHONY: all clean
