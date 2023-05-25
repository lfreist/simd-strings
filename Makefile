NAME = simdstr
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -fPIC -I./include
LIBS = -lm

SRC_DIR = src
OBJ_DIR = obj

HEADERS = $(wildcard include/simdstr/*.h)
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

STATIC_LIB = $(NAME).a
DYNAMIC_LIB = $(NAME).so

all: dirs compile

dirs:
	mkdir -p $(OBJ_DIR)

compile: $(STATIC_LIB) $(DYNAMIC_LIB)

$(STATIC_LIB): $(OBJECTS)
	ar rcs $@ $^

$(DYNAMIC_LIB): $(OBJECTS)
	$(CC) -shared -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

install: $(STATIC_LIB) $(DYNAMIC_LIB)
	install -d /usr/local/lib /usr/local/include
	install -m 644 $(STATIC_LIB) /usr/local/lib
	install -m 755 $(DYNAMIC_LIB) /usr/local/lib
	install -m 644 include/simdstr /usr/local/include

clean:
	rm -f $(OBJECTS) $(STATIC_LIB) $(DYNAMIC_LIB)