import * as THREE from 'three';

const scene = new THREE.Scene();
const camera = new THREE.PerspectiveCamera( 75, window.innerWidth / window.innerHeight, 0.1, 1000 );

const renderer = new THREE.WebGLRenderer();
renderer.setSize( window.innerWidth, window.innerHeight );
document.body.appendChild( renderer.domElement );

var geometry = new THREE.SphereGeometry(1, 100, 100);
const texture = new THREE.TextureLoader().load("/img/earthmap1k.jpg");
const material = new THREE.MeshPhongMaterial( { map:texture } );
material.bumpMap = new THREE.TextureLoader().load('/img/earthbump1k.jpg');
material.bumpScale = 5;
material.specularMap = new THREE.TextureLoader().load('/img/earthspec1k.jpg')
material.specular  = new THREE.Color('grey')
var earthmesh = new THREE.Mesh(geometry, material);

var cloudG   = new THREE.SphereGeometry(1.01, 32, 32)
var cloudM  = new THREE.MeshPhongMaterial({
  map         : new THREE.TextureLoader().load('/img/fair_clouds_4k.png'),
  side        : THREE.DoubleSide,
  opacity     : 0.8,
  transparent : true,
  depthWrite  : false,
});
var cloudMesh = new THREE.Mesh(cloudG, cloudM)
earthmesh.add(cloudMesh)

const light = new THREE.PointLight(0xFFFFFF, 100, 100);
light.position.set(5, 0, 5);
scene.add( light );
scene.add( earthmesh );

camera.position.z = 3;

function animate() {
	requestAnimationFrame( animate );

	earthmesh.rotation.y -= 0.01;

	renderer.render( scene, camera );
}

animate();