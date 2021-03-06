#include "visualizers.h"
#if GFX_ENABLED

#define DEPTH_CONTRACT_SPD 2.0
#define DEPTH_EXPAND_SPD 4.0
#define DEPTH_GRACE_TIME 0.5

namespace {
	struct _ : public Visualizer {
		_() {
			name = "Basic Visualizer";
		}

		//vector<double> depthTimerVec;
		float maxDepth = 1.f;
		float graceTime = 0.f;

		void prepare(VisualizerListInfo* inf, List<int>* mainList, double elapsedSeconds) {
			if (inf->maxDepth >= (int)ceil(maxDepth)) {
				maxDepth = fmin(maxDepth + (float)((double)inf->maxDepth * elapsedSeconds * DEPTH_CONTRACT_SPD), inf->maxDepth);
				graceTime = 0.f;
			}
			else {
				if (graceTime < DEPTH_GRACE_TIME) {
					graceTime += (float)elapsedSeconds;
					double ovr = (double)graceTime - DEPTH_GRACE_TIME;
					elapsedSeconds = ovr;
				}
				if (graceTime >= DEPTH_GRACE_TIME)
					maxDepth = fmax(maxDepth - (float)((double)maxDepth * elapsedSeconds * DEPTH_EXPAND_SPD), inf->maxDepth);
			}
			/*if (inf->maxDepth > maxDepth) {
				maxDepth = inf->maxDepth;
				depthTimerVec.clear();
				depthTimerVec.resize(maxDepth, 0.0);
			}

			for (int i = 0; i < maxDepth; i++) {
				auto& t = depthTimerVec[i];
				if (i < inf->maxDepth - 1) {
					t = 0.0;
					continue;
				}
				t += elapsedSeconds;
				if (t > DEPTH_GRACE_TIME)
					maxDepth = i + 1;
			}*/
		}

		bool drawList(VisualizerListInfo* inf, List<int>* l, int depthInd, int depth) { return depth < maxDepth; }

		void drawElement(VisualizerListInfo* inf, List<int>* l, int depthInd, ListElement<int>* e, int pos, int depth, bool marked) {
			float width = 2.f / inf->realSize;
			float height = (float)e->val / inf->realSize * 2.f / maxDepth;
			float x = ((float)l->offset + (float)pos) / inf->realSize * 2.f - 1.f + width / 2.f;
			float y = -1.f + (float)depth / maxDepth * 2.f + height / 2.f;
			vec4 col = marked ? vec4(0.f, 0.f, 0.f, 1.f) : colHSV((float)e->val / inf->realSize, 1.f, 1.f);
			drawShape(shape_square, vec3(x, y, 0.f), vec3(width, height, 1.f), vec3(), col);
		}
	}_;
}

Visualizer* visualizer_basic = &::_;

#endif