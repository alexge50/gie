const { app, BrowserWindow } = require('electron');
var path = require('path');
var fs = require('fs');

let win;

function createWindow () {
    win = new BrowserWindow({ width: 800, height: 600 });

    const startUrl = process.env.ELECTRON_START_URL || url.format({
        pathname: path.join(__dirname, '/../build/index.html'),
        protocol: 'file:',
        slashes: true
    });

    win.loadURL(startUrl);

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
