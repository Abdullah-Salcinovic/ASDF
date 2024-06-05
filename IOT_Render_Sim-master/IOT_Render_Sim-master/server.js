const express = require("express");
const path = require("path");
const bodyParser = require("body-parser");

const app = express();
const port = 3000;
const hostname = "localhost";
const ip = "192.168.1.203";

// Serve static files from the "public" directory
app.use(express.static(path.join(__dirname, "public")));
app.use(bodyParser.json({ limit: "50mb" }));

// Serve node_modules directory
app.use('/node_modules', express.static(path.join(__dirname, 'node_modules')));

// Send index.html for the root URL
app.get("/", (req, res) => {
    res.sendFile(path.join(__dirname, "public", "index.html"));
});

app.get("/status", (req, res) => { console.log("Ass"); });

app.post("/api", (req, res) => {
    req.on("data", (data) => {
        console.log(data.toString());
    });
});

// Handle image data
app.post('/api/image', (req, res) => {
    console.log('Received image data:');
    console.log(req.body);
    res.sendStatus(200);
});

// Handle sound data
app.post('/api/sound', (req, res) => {
    console.log('Received sound data:');
    console.log(req.body);
    res.sendStatus(200);
});

// Handle mesh data
app.post('/api/mesh', (req, res) => {
    console.log('Received mesh data:');
    console.log(req.body);
    res.sendStatus(200);
});

// Handle bone data
app.post('/api/bone', (req, res) => {
    console.log('Received bone data:');
    console.log(req.body);
    res.sendStatus(200);
});

// Handle animation data
app.post('/api/track', (req, res) => {
    console.log('Received animation data:');
    console.log(req.body);
    res.sendStatus(200);
});

// Start the server
app.listen(port, hostname, () => {
    console.log(`Server running at http://${hostname}:${port}/`);
});

app.listen(port, ip, () => {
    console.log(`Server running at http://${ip}:${port}/`);
});

// Handle errors
app.use((req, res, next) => {
    res.status(404).send("Sorry can't find that!");
});

