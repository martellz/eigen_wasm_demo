# eigen wasm demo

## eigen

eigen是一个 C++ 开源线性代数库(其他库的使用方法应该类似)，我们需要下载源码，放到项目目录下

```bash

wget https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.zip

upzip eigen-3.4.0.zip

mv eigen-3.4.0 eigen

```

## emscripten

install emscripten and activate it

## helloWorld module

### 编译

```bash

emcc --bind -o helloWorld.js -s MODULARIZE -s EXPORT_NAME=HelloWorldModule src/helloWorld.cpp -I ./eigen/ -s SINGLE_FILE=1

```

### 使用

example code:

```javascript

import HelloWorldModule from './helloWorld.js'

foo() {
    HelloWorldModule().then((m) => {
        // those functions are implemented in src/helloWorld.cpp
        m.testEigen();
        m.benchmarkEigen();
    });
}

```

## class LinearPredictor

### 编译

emcc --bind -o linearPredictor.js -s MODULARIZE -s EXPORT_NAME=LinearPredictor  src/tracker/linear_predictor.cpp  -I ./eigen/ -s SINGLE_FILE=1

### 使用

TODO

### notice!
embind 还有点问题，还不能在web里正常使用
