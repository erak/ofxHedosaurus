#include "SkinSource.h"

#include <constants.h>
#include <midi.h>
#include <algorithms.h>

#include <boost/optional.hpp>

//-----------------------------------------------------------------------------

using namespace engine;

SkinSource::SkinSource()
	: node0_(new file_node("assets/skin_tex_00.png"))
	, node1_(new file_node("assets/skin_tex_01.png"))
	, node2_(new file_node("assets/skin_tex_02.png"))
	, node3_(new file_node("assets/skin_tex_03.png"))
	, node4_(new file_node("assets/skin_tex_04.png"))
	, node5_(new file_node("assets/skin_tex_05.png"))
	, node6_(new file_node("assets/skin_tex_06.png"))
	, node7_(new file_node("assets/skin_tex_07.png"))

{
	name = "Skin";
	allocate(QUAD_WIDTH, QUAD_HEIGHT);

	graph_.add_input(node0_);
	nodes_.push_back(node0_);

	graph_.add_input(node1_);
	nodes_.push_back(node1_);

	graph_.add_input(node2_);
	nodes_.push_back(node2_);

	graph_.add_input(node3_);
	nodes_.push_back(node3_);

	graph_.add_input(node4_);
	nodes_.push_back(node4_);

	graph_.add_input(node5_);
	nodes_.push_back(node5_);

	graph_.add_input(node6_);
	nodes_.push_back(node6_);

	graph_.add_input(node7_);
	nodes_.push_back(node7_);

	modulation mod2{node2_};
	modulator mod_fn2 {"alpha", [](ofParameter<size_t>& param) {
		auto value = param.get();
        value = value == 255 ? 0 : 255;
        param.set(value);
	}};
	mod2.add_modulator(std::move(mod_fn2));
	modulations_.push_back(std::move(mod2));
}

//-----------------------------------------------------------------------------

void SkinSource::setup()
{
}

//-----------------------------------------------------------------------------

void SkinSource::update()
{
	graph_.update();
}

//-----------------------------------------------------------------------------

void SkinSource::draw()
{
	ofBackground(0); // this matters
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	
	// reduce visualstd::vector<modulation> modulations_; strength / impact
	// ofSetColor(255, 255, 255, 150);

	graph_.draw();
}

//-----------------------------------------------------------------------------

void SkinSource::set_param(const size_t& node_idx,
			   const std::string& name, const size_t& value)
{
	nodes_.at(node_idx)->parameters().get<size_t>(name).set(value * 2);
}

//-----------------------------------------------------------------------------

void SkinSource::modulate()
{
	for (auto& modulation: modulations_)
    	modulation.step();
}