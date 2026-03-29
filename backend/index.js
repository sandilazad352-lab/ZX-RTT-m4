const express = require('express');
const os = require('os');
const app = express();

app.use(express.json());
app.use(express.text({ type: 'application/json' }));
app.use(express.text({ type: 'text/plain' }));

// Middleware to parse text as JSON
app.use((req, res, next) => {
    if (typeof req.body === 'string') {
        try {
            req.body = JSON.parse(req.body);
        } catch (e) {
            console.warn('[WARNING] Failed to parse body as JSON:', req.body);
        }
    }
    next();
});

const items = [
    {
        id: 1,
        name: 'Item 0',
        description: 'This is a sample item',
        value: 100,
        timestamp: new Date().toISOString()
    }
];
let nextId = 2;  // Start from 2 since first item has ID 1

// CREATE - POST /api/items
app.post('/api/items', (req, res) => {
    // Validate required fields
    if (!req.body || !req.body.name || req.body.name.trim() === '') {
        console.error('[ERROR] POST /api/items - Missing or empty name field:', req.body);
        return res.status(400).json({ error: 'Missing required field: name', received: req.body });
    }

    const newItem = {
        id: nextId++,
        name: req.body.name.trim(),
        description: req.body.description || '',
        value: parseInt(req.body.value) || 0,
        timestamp: new Date().toISOString()
    };
    
    items.push(newItem);
    console.log('[CREATE] Item created:', JSON.stringify(newItem));
    res.status(201).json(newItem);
});

// READ - GET /api/items/:id
app.get('/api/items/:id', (req, res) => {
    const item = items.find(i => i.id === parseInt(req.params.id));
    if (!item) {
        console.warn('[WARN] GET /api/items/:id - Item not found:', req.params.id);
        return res.status(404).json({ error: 'Not found' });
    }
    console.log('[READ] Item retrieved:', JSON.stringify(item));
    res.json(item);
});

// LIST - GET /api/items
app.get('/api/items', (req, res) => {
    console.log('[LIST] Returning', items.length, 'items');
    res.json(items);
});

// UPDATE - PUT /api/items/:id
app.put('/api/items/:id', (req, res) => {
    const item = items.find(i => i.id === parseInt(req.params.id));
    if (!item) {
        console.warn('[WARN] PUT /api/items/:id - Item not found:', req.params.id);
        return res.status(404).json({ error: 'Not found' });
    }
    
    // Update fields if provided
    if (req.body.name && req.body.name.trim()) item.name = req.body.name.trim();
    if (req.body.description !== undefined) item.description = req.body.description;
    if (req.body.value !== undefined) item.value = parseInt(req.body.value) || item.value;
    
    console.log('[UPDATE] Item updated:', JSON.stringify(item));
    res.json(item);
});

// DELETE - DELETE /api/items/:id
app.delete('/api/items/:id', (req, res) => {
    const idx = items.findIndex(i => i.id === parseInt(req.params.id));
    if (idx === -1) {
        console.warn('[WARN] DELETE /api/items/:id - Item not found:', req.params.id);
        return res.status(404).json({ error: 'Not found' });
    }
    
    const deleted = items.splice(idx, 1);
    console.log('[DELETE] Item deleted:', JSON.stringify(deleted[0]));
    res.json({ success: true });
});

const PORT = 3000;
const HOST = '0.0.0.0';

// Get local IP address
function getLocalIP() {
    const interfaces = os.networkInterfaces();
    for (const name of Object.keys(interfaces)) {
        for (const iface of interfaces[name]) {
            if (iface.family === 'IPv4' && !iface.internal) {
                return iface.address;
            }
        }
    }
    return 'localhost';
}

app.listen(PORT, HOST, () => {
    const localIP = getLocalIP();
    console.log(`API listening on http://${localIP}:${PORT}`);
});