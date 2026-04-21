// 15 Fixed Cities with Coordinates (scaled for 1200x800 map)
const cities = {
    'Delhi': { x: 150, y: 80 },
    'Jaipur': { x: 200, y: 150 },
    'Lucknow': { x: 350, y: 120 },
    'Patna': { x: 500, y: 140 },
    'Guwahati': { x: 650, y: 100 },
    'Ahmedabad': { x: 140, y: 280 },
    'Indore': { x: 280, y: 320 },
    'Bhopal': { x: 350, y: 280 },
    'Nagpur': { x: 420, y: 350 },
    'Raipur': { x: 480, y: 420 },
    'Mumbai': { x: 120, y: 420 },
    'Pune': { x: 200, y: 480 },
    'Kolkata': { x: 600, y: 280 },
    'Ranchi': { x: 520, y: 250 },
    'Bhubaneswar': { x: 600, y: 350 }
};

// Graph edges with distances
const edges = [
    { from: 'Delhi', to: 'Jaipur', distance: 280 },
    { from: 'Delhi', to: 'Lucknow', distance: 480 },
    { from: 'Jaipur', to: 'Ahmedabad', distance: 540 },
    { from: 'Ahmedabad', to: 'Mumbai', distance: 520 },
    { from: 'Mumbai', to: 'Pune', distance: 150 },
    { from: 'Pune', to: 'Indore', distance: 580 },
    { from: 'Indore', to: 'Bhopal', distance: 190 },
    { from: 'Bhopal', to: 'Nagpur', distance: 350 },
    { from: 'Nagpur', to: 'Raipur', distance: 280 },
    { from: 'Raipur', to: 'Bhubaneswar', distance: 520 },
    { from: 'Bhubaneswar', to: 'Kolkata', distance: 440 },
    { from: 'Kolkata', to: 'Patna', distance: 580 },
    { from: 'Patna', to: 'Lucknow', distance: 520 },
    { from: 'Patna', to: 'Ranchi', distance: 320 },
    { from: 'Ranchi', to: 'Raipur', distance: 470 },
    { from: 'Raipur', to: 'Bhopal', distance: 620 },
    { from: 'Lucknow', to: 'Patna', distance: 400 },
    { from: 'Patna', to: 'Guwahati', distance: 800 },
    { from: 'Guwahati', to: 'Kolkata', distance: 550 }
];

let currentPath = [];
let currentDistance = 0;

// Initialize on page load
document.addEventListener('DOMContentLoaded', () => {
    populateCityDropdowns();
    renderMap();
    setupEventListeners();
});

// Populate city dropdowns
function populateCityDropdowns() {
    const fromSelect = document.getElementById('from-city');
    const toSelect = document.getElementById('to-city');
    const cityList = Object.keys(cities).sort();
    
    cityList.forEach(city => {
        const option1 = document.createElement('option');
        option1.value = city;
        option1.textContent = city;
        fromSelect.appendChild(option1);
        
        const option2 = document.createElement('option');
        option2.value = city;
        option2.textContent = city;
        toSelect.appendChild(option2);
    });
}

// Render the fixed map
function renderMap() {
    const mapContainer = document.getElementById('map');
    const width = mapContainer.clientWidth || 1200;
    const height = mapContainer.clientHeight || 800;
    
    // Create SVG
    const svg = d3.select('#map')
        .html('') // Clear any existing content
        .append('svg')
        .attr('width', width)
        .attr('height', height)
        .attr('viewBox', `0 0 1000 700`)
        .style('background', 'white');
    
    // Draw edges first (so they're behind nodes)
    edges.forEach(edge => {
        const fromCity = cities[edge.from];
        const toCity = cities[edge.to];
        
        svg.append('line')
            .attr('class', 'edge')
            .attr('x1', fromCity.x)
            .attr('y1', fromCity.y)
            .attr('x2', toCity.x)
            .attr('y2', toCity.y)
            .attr('id', `edge-${edge.from}-${edge.to}`);
        
        // Distance label
        const midX = (fromCity.x + toCity.x) / 2;
        const midY = (fromCity.y + toCity.y) / 2;
        
        svg.append('text')
            .attr('class', 'edge-label')
            .attr('x', midX)
            .attr('y', midY - 8)
            .text(edge.distance + 'km');
    });
    
    // Draw nodes
    Object.entries(cities).forEach(([cityName, coords]) => {
        svg.append('circle')
            .attr('class', 'city-node')
            .attr('cx', coords.x)
            .attr('cy', coords.y)
            .attr('r', 12)
            .attr('id', `node-${cityName}`)
            .on('mouseover', function() {
                d3.select(this).attr('r', 16);
            })
            .on('mouseout', function() {
                d3.select(this).attr('r', 12);
            });
        
        // City label
        svg.append('text')
            .attr('class', 'city-label')
            .attr('x', coords.x)
            .attr('y', coords.y + 30)
            .text(cityName);
    });
}

// Setup event listeners
function setupEventListeners() {
    document.getElementById('find-route-btn').addEventListener('click', findRoute);
    document.getElementById('reset-btn').addEventListener('click', resetMap);
}

// Find route via backend
async function findRoute() {
    const fromCity = document.getElementById('from-city').value;
    const toCity = document.getElementById('to-city').value;
    
    if (!fromCity || !toCity) {
        alert('Please select both source and destination cities');
        return;
    }
    
    if (fromCity === toCity) {
        alert('Source and destination must be different');
        return;
    }
    
    try {
        const response = await fetch(`http://localhost:8080/route?from=${encodeURIComponent(fromCity)}&to=${encodeURIComponent(toCity)}`);
        const data = await response.json();
        
        if (data.success) {
            currentPath = data.path;
            currentDistance = data.distance;
            highlightRoute();
            displaySummary();
        } else {
            alert('Error: ' + data.error);
        }
    } catch (error) {
        alert('Error connecting to backend: ' + error.message);
    }
}

// Highlight the route on map
function highlightRoute() {
    const svg = d3.select('svg');
    
    // Reset all edges
    svg.selectAll('.edge').classed('shortest-path', false);
    
    // Highlight path edges
    for (let i = 0; i < currentPath.length - 1; i++) {
        const fromCity = currentPath[i];
        const toCity = currentPath[i + 1];
        
        // Check both directions
        let edgeId = `edge-${fromCity}-${toCity}`;
        if (svg.select(`#${edgeId}`).empty()) {
            edgeId = `edge-${toCity}-${fromCity}`;
        }
        
        svg.select(`#${edgeId}`).classed('shortest-path', true);
    }
}

// Display route summary
function displaySummary() {
    const pathDisplay = currentPath.join(' → ');
    document.getElementById('total-distance').textContent = currentDistance + ' km';
    document.getElementById('route-path').textContent = pathDisplay;
}

// Reset map
function resetMap() {
    document.getElementById('from-city').value = '';
    document.getElementById('to-city').value = '';
    document.getElementById('total-distance').textContent = '—';
    document.getElementById('route-path').textContent = '—';
    
    const svg = d3.select('svg');
    svg.selectAll('.edge').classed('shortest-path', false);
    
    currentPath = [];
    currentDistance = 0;
}
