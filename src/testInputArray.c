#include <emscripten.h>

// 定义一个处理数组的函数
int squareNumber(int num) {
    return num * num;
}

// 定义一个可以在JavaScript中调用的函数
EMSCRIPTEN_KEEPALIVE
int* processArray(int* inputArray, int length) {
    // 为输出数组分配内存
    int* outputArray = (int*) malloc(length * sizeof(int));

    // 处理输入数组，并将结果存储在输出数组中
    for (int i = 0; i < length; i++) {
        outputArray[i] = squareNumber(inputArray[i]);
    }

    // 返回处理后的数组
    return outputArray;
}


/*

// 在浏览器中打开编译的html， 在 console 中使用下面的js代码

let inputArray = [1, 2, 3, 4, 5];

// 将输入数组转换为C中的格式（使用EM_ASM将其传递给C）
let inputArrayPointer = Module._malloc(inputArray.length * Int32Array.BYTES_PER_ELEMENT);
Module.HEAP32.set(inputArray, inputArrayPointer / Int32Array.BYTES_PER_ELEMENT);

// 调用C函数来处理输入数组（使用Module.ccall）
const outputArrayPointer = Module.ccall('processArray', 'number', ['number', 'number'], [inputArrayPointer, inputArray.length]);

// 获取处理后的数组
let outputArray =  Module.HEAP32.subarray(outputArrayPointer / Int32Array.BYTES_PER_ELEMENT, outputArrayPointer / Int32Array.BYTES_PER_ELEMENT + inputArray.length);

// 将输出数组从C格式转换为JavaScript格式（使用Array.from）
const outputJsArray = Array.from(outputArray);
console.log(outputJsArray);

Module._free(inputArrayPointer);
Module._free(outputArrayPointer);

*/