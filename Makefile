C_FLAGS_LINUX   = -D _DEBUG -ggdb3 -std=c++2a -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-check -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,leak,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
C_FLAGS_WINDOWS = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

windows: akinator_windows logging_windows console_settings_windows
	g++ Akinator/main.cpp Obj/Akinator.o Obj/Logging.o Obj/ConsoleSettings.o $(C_FLAGS_WINDOWS) -o Exe/Start.exe

akinator_windows: logging_windows console_settings_windows
	g++ -c Akinator/Akinator.cpp Obj/Logging.o $(C_FLAGS_WINDOWS) -o Obj/Akinator.o

logging_windows: console_settings_windows
	g++ -c Akinator/Libs/Logging/Logging.cpp $(C_FLAGS_WINDOWS) -o Obj/Logging.o

console_settings_windows:
	g++ -c Akinator/Libs/ConsoleSettings/ConsoleSettings.cpp $(C_FLAGS_WINDOWS) -o Obj/ConsoleSettings.o


linux: akinator_linux logging_linux console_settings_linux
	g++ Akinator/main.cpp Obj/Akinator.o Obj/Logging.o Obj/ConsoleSettings.o $(C_FLAGS_LINUX) -o Exe/Start

akinator_linux: logging_linux
	g++ -c Akinator/Akinator.cpp Obj/Logging.o $(C_FLAGS_LINUX) -o Obj/Akinator.o

logging_linux: console_settings_linux
	g++ -c Akinator/Libs/Logging/Logging.cpp $(C_FLAGS_LINUX) -o Obj/Logging.o

console_settings_linux:
	g++ -c Akinator/Libs/ConsoleSettings/ConsoleSettings.cpp $(C_FLAGS_LINUX) -o Obj/ConsoleSettings.o

clean_linux:
	find . -name "*.o" -delete