//--post-js file creates a promise allowing us to safely use it in other files

Module.ready = new Promise(function(resolve, reject) {
    addOnPreMain(function() {
        var api = {
            import_model: Module.cwrap('import_model', null, ['number','number']),
            export_model: Module.cwrap('export_model', 'number', ['string']),
            set_camera: Module.cwrap('set_camera',null,['number','number','number','number','number','number']),
            on_mouse_up: Module.cwrap('on_mouse_up', null, ['number']),
            get_export_strlen: Module.cwrap('get_export_strlen', 'number', ['number']),
            translate_vertex: Module.cwrap('translate_vertex', null, null),
            scale: Module.cwrap('scale',null,['number']),
            import_file: Module.cwrap('import_file', null, ['string'], ['number']),
            undo: Module.cwrap('undo',null),
            redo: Module.cwrap('redo',null)
        };
        resolve(api);
    });
});



