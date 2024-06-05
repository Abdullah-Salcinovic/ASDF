// Import necessary Three.js classes
import * as THREE from '/node_modules/three/build/three.module.js';

// Create the scene
const scene = new THREE.Scene();

// Create a camera, which determines what we'll see when we render the scene
const camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);

// Create a renderer and attach it to our document
const renderer = new THREE.WebGLRenderer();
renderer.setSize(window.innerWidth, window.innerHeight);
document.body.appendChild(renderer.domElement);

// Create a geometry (a cube)
const geometry = new THREE.BoxGeometry();

// Create a material and assign a color
const material = new THREE.MeshBasicMaterial({ color: 0x00ff00 });

// Create a mesh, which is the combination of a geometry and a material
const cube = new THREE.Mesh(geometry, material);

// Add the cube to the scene
scene.add(cube);

// Move the camera away from the origin, down the Z axis
camera.position.z = 5;

// Create a function to animate our scene
function animate() {
    requestAnimationFrame(animate);

    // Rotate the cube for some basic animation
    cube.rotation.x += 0.01;
    cube.rotation.y += 0.01;

    // Render the scene from the perspective of the camera
    renderer.render(scene, camera);
}

// Run the animation function for the first time to kick things off
animate();
