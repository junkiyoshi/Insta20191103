#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetLineWidth(1.5);

	this->font.loadFont("fonts/Kazesawa-Bold.ttf", 400, true, true, true);
	this->mesh.setMode(OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->mesh.clear();

	int hue = 0;
	ofColor color;

	string word = "35";
	auto word_size = glm::vec2(this->font.stringWidth(word), this->font.stringHeight(word));

	vector<ofPath> word_path = this->font.getStringAsPoints(word, true, false);
	for (int word_index = 0; word_index < word.size(); word_index++) {

		vector<ofPolyline> outline = word_path[word_index].getOutline();
		for (int outline_index = 0; outline_index < outline.size(); outline_index++) {

			outline[outline_index] = outline[outline_index].getResampledBySpacing(20);
			vector<glm::vec3> vertices = outline[outline_index].getVertices();
			for (int vertices_index = 0; vertices_index < vertices.size(); vertices_index++) {

				auto vertex = vertices[vertices_index];
				vertex += glm::vec3(word_size.x * -0.5, word_size.y * 0.5, 0);

				this->mesh.addVertex(vertex);
				
				color.setHsb((hue += 10) % 255, 180, 255);
				this->mesh.addColor(color);
			}
		}
	}

	for (int i = 0; i < 230; i++) {

		auto location = glm::vec3(
			ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.0005), 0, 1, -360, 360),
			ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.0005), 0, 1, -360, 360),
			0);

		this->mesh.addVertex(location);

		color.setHsb((hue += 10) % 255, 180, 255);
		this->mesh.addColor(color);
	}

	for (int i = 0; i < this->mesh.getVertices().size(); i++) {

		auto location = this->mesh.getVertex(i);
		vector<int> near_index_list;
		for (int k = 0; k < this->mesh.getVertices().size(); k++) {

			if (i == k) { continue; }

			auto other = this->mesh.getVertex(k);
			auto distance = glm::distance(location, other);
			if (distance < 28) {

				this->mesh.addIndex(i);
				this->mesh.addIndex(k);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);

	this->mesh.draw();
	for (int i = 0; i < this->mesh.getVertices().size(); i++) {

		auto location = this->mesh.getVertex(i);
		auto near_count = 3;
		for (int k = 0; k < this->mesh.getVertices().size(); k++) {

			if (i == k) { continue; }

			auto other = this->mesh.getVertex(k);
			auto distance = glm::distance(location, other);
			if (distance < 23) {

				near_count++;
			}
		}

		ofSetColor(this->mesh.getColor(i));
		ofDrawCircle(location, near_count);
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}