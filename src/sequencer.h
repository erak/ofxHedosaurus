#pragma once

#include <constants.h>
#include <render_graph.h>

#include <ofMain.h>

namespace engine {

// ----------------------------------------------------------------------------

namespace util {

inline size_t bpm_to_interval_ms(const size_t bpm)
{
	auto bpms = bpm / 60.f / 1000.f;
    return std::ceil( 1.f / bpms );
}

} // util

//-----------------------------------------------------------------------------

class modulation {
public:
    modulation(node_ptr node) : node_(node) {}

    void step()
    {
        auto value = node_->parameters().get<size_t>("alpha").get();
        value = value == 255 ? 0 : 255;
        node_->parameters().get<size_t>("alpha").set(value);
    }

private:
    node_ptr node_;
};

//-----------------------------------------------------------------------------

class sequencer {
public:
    sequencer()
    {
        parameters_.add(bpm_.set("bpm", 60));
        parameters_.add(running_.set("running", false));
    }

    void update()
    {
        auto time = ofGetElapsedTimeMillis();
        auto interval = util::bpm_to_interval_ms(bpm_.get());

        time_delta_ += time - prev_time_;
        
        if (time_delta_ > interval)
        {
            step();
            time_delta_ = 0;
        }

        prev_time_ = time;
    }

    void step()
    {
        if (running_.get())
        {
            for (auto& modulation: modulations_)
                modulation.step();
        }
    }

    void add_modulation(modulation modulation)
    {
        modulations_.push_back(modulation);
    }

    ofParameterGroup& parameters() { return parameters_; }

private:
    size_t prev_time_ = 0;
    size_t time_delta_ = 0;

    ofParameterGroup parameters_;
    ofParameter<size_t> bpm_;
    ofParameter<bool> running_;

    std::vector<modulation> modulations_;
};

} // engine