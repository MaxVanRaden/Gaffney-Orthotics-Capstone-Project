//--post-js file creates a promise allowing us to safely use it in other files

Module.ready = new Promise(function(resolve, reject) {
    addOnPreMain(function() {
        // Refer to the below for documentation on cwrap arguments
        // https://emscripten.org/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html#calling-compiled-c-functions-from-javascript-using-ccall-cwrap
        var api = {
            import_model: Module.cwrap('import_model', 'char* str', ['char* str'], 'int_len', ['int_len'], 'target', ['target']),
            export_model: Module.cwrap('export_model', 'number', ['number','number']),
            export_new: Module.cwrap('export_new', 'number', ['number', 'number']),
            get_export_strlen: Module.cwrap('get_export_strlen', 'number')
        };
        resolve(api);
    });
});