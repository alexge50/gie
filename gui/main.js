const { app, BrowserWindow } = require('electron');
var path = require('path');
var fs = require('fs');

let win;

function createWindow () {
    win = new BrowserWindow({ width: 800, height: 600 });
    win.loadFile('views/index.html');
    win.webContents.openDevTools();
    win.on('closed', () => {
        win = null
    })
}

app.on('ready', createWindow);
app.on('window-all-closed', () => {
    if (process.platform !== 'darwin') {
        app.quit()
    }
});

app.on('activate', () => {
    if (win === null) {
        createWindow()
    }
});