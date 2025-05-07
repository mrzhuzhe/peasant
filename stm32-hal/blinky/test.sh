export STM32_TOOLCHAIN_PATH="$HOME/opt/cross-arm"
export STM32_TARGET_TRIPLET=arm-none-eabi
export STM32_CUBE_F1_PATH="/home/sanszhu/code/code/STM32CubeF1"

cmake -B build -S . --graphviz=foo.dot
cmake --build build 

# dot -Tpng -o ./outputs/foo.png ./outputs/foo.dot
# dot -Tpng -o ./outputs/foo.dot.CMSISSTM32F1.dependers.png ./outputs/foo.dot.CMSISSTM32F1.dependers
# dot -Tpng -o ./outputs/foo.dot.STM32F1.dependers.png ./outputs/foo.dot.STM32F1.dependers