#include <opencv2/opencv.hpp>
#include "ElementaryElements.h"

namespace GeoChain {
namespace Euclidean {

#define COLOR_WHITE cv::Scalar(255, 255, 255)
#define COLOR_BLACK cv::Scalar(0, 0, 0)
#define COLOR_RED cv::Scalar(34, 34, 178)
#define COLOR_BLUE cv::Scalar(139, 78, 16)
#define COLOR_YELLOW cv::Scalar(205, 250, 255)
#define COLOR_GREEN cv::Scalar(193, 255, 193)

#define COLOR_SPACE cv::Scalar(200, 200, 200)
#define COLOR_NOTATION cv::Scalar(50, 50, 50)

static void MouseLocator(int event, int x, int y, int flag, void *param) {
	// readin the IMAGE ptr and form an IMAGE
	// cv::Mat img_ = *(cv::Mat *)(param);
	cv::Point *pt_ = (cv::Point *)(param);
	std::string s_;

	if (event == CV_EVENT_LBUTTONDOWN) {
		// s_ = '(' + std::to_string(x) + ',' + std::to_string(y) + ')';
		// cv::putText(img_, s_, cv::Point(0, 0), CV_FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(0, 255, 0), 2, 8);
		pt_->x = x;
		pt_->y = y;
	}
	// else if (event == CV_EVENT_LBUTTONUP) {
	// 	pt_->x = -g_GlobalVars.convention_error_code;
	// 	pt_->y = -g_GlobalVars.convention_error_code;
	// }
	else if (flag & CV_EVENT_FLAG_LBUTTON) {
		pt_->x = x;
		pt_->y = y;
	} else {	// if (event == CV_EVENT_MOUSEMOVE)
		pt_->x = -x;
		pt_->y = -y;
	}
}

static std::string StringFloatPrecision(std::string value, int p) {
	std::string::size_type position;
	position = value.find("-");
	if (position == value.npos) {
		value = "+" + value;
	}
	position = value.find(".");
	if (position == value.npos)
		return value;
	else
		return value.substr(0, position + p + 1);
}

class Visualizer2D {
 public:
	kDimension dim_ = EUC2D;
	int spacer_;

	// scaling parameter
	float standardize_;
	std::vector<Point> attendants_;

	// canvas
	cv::Mat canvas_;

	Visualizer2D(std::vector<Point> &attendants, float standardize, int spacer)
			: attendants_(attendants), standardize_(standardize), spacer_(spacer) {
		for (auto &&t_attendant : this->attendants_) {
			if (t_attendant.dim_ != this->dim_) {
				LOG(ERROR) << "there exists at least one attendant Point dim not matched with Visualizer!";
				break;
			}
			if (t_attendant.x_ < this->min_x_) {
				this->min_x_ = t_attendant.x_;
			}
			if (t_attendant.x_ > this->max_x_) {
				this->max_x_ = t_attendant.x_;
			}
			if (t_attendant.y_ < this->min_y_) {
				this->min_y_ = t_attendant.y_;
			}
			if (t_attendant.y_ > this->max_y_) {
				this->max_y_ = t_attendant.y_;
			}
		}
		this->scale_x_ = this->max_x_ - this->min_x_;
		this->scale_y_ = this->max_y_ - this->min_y_;
		// LOG(INFO) << this->min_x_ << " " << this->max_x_ << " " << this->max_y_ << " " << this->min_y_;
		if (std::min(this->scale_x_, this->scale_y_) < 0.0) {
			LOG(WARNING) << "no attendant Point initiated with Visualizer!";
			this->scaling_ = 1.0;
			this->pixel_width = this->spacer_ * 2 + 1;
			this->pixel_height = this->spacer_ * 2 + 1;
		} else if (std::max(this->scale_x_, this->scale_y_) == 0.0) {
			LOG(WARNING) << "Point attendants superpositioned!";
			this->scaling_ = 1.0;
			this->shift_x_ = -std::min(this->min_x_, float(0.0)) + float(this->spacer_);
			this->shift_y_ = -std::min(this->min_y_, float(0.0)) + float(this->spacer_);
			this->pixel_width = std::ceil(this->max_x_ + this->shift_x_) + this->spacer_ + 1;
			this->pixel_height = std::ceil(this->max_y_ + this->shift_y_) + this->spacer_ + 1;
		} else {
			this->scaling_ = this->standardize_ / std::max(this->scale_x_, this->scale_y_);
			this->shift_x_ = -std::min(this->min_x_ * this->scaling_, float(0.0)) + float(this->spacer_);
			this->shift_y_ = -std::min(this->min_y_ * this->scaling_, float(0.0)) + float(this->spacer_);
			this->pixel_width = std::ceil(this->max_x_ * this->scaling_ + this->shift_x_) + this->spacer_ + 1;
			this->pixel_height = std::ceil(this->max_y_ * this->scaling_ + this->shift_y_) + this->spacer_ + 1;
		}
		this->shift_.x = this->shift_x_;
		this->shift_.y = this->shift_y_;
	};
	~Visualizer2D(void){};

	void Init(bool grid_on = false) {
		// LOG(INFO) << this->pixel_height << " " << this->pixel_width;
		this->canvas_ = cv::Mat(this->pixel_height, this->pixel_width, CV_8UC3, COLOR_SPACE);
		cv::Point2f origin_(this->shift_x_, this->shift_y_);
		cv::Point2f x_axis_neg_infty(this->shift_x_ - g_GlobalVars.convention_infinity * this->scaling_, this->shift_y_);
		cv::Point2f x_axis_pos_infty(this->shift_x_ + g_GlobalVars.convention_infinity * this->scaling_, this->shift_y_);
		cv::Point2f y_axis_neg_infty(this->shift_x_, this->shift_y_ - g_GlobalVars.convention_infinity * this->scaling_);
		cv::Point2f y_axis_pos_infty(this->shift_x_, this->shift_y_ + g_GlobalVars.convention_infinity * this->scaling_);
		cv::line(this->canvas_, x_axis_neg_infty, x_axis_pos_infty, COLOR_NOTATION, g_GlobalVars.visualize_line_width);
		cv::line(this->canvas_, y_axis_neg_infty, y_axis_pos_infty, COLOR_NOTATION, g_GlobalVars.visualize_line_width);
		cv::circle(this->canvas_, origin_, g_GlobalVars.visualize_point_size, COLOR_BLACK, -1);
	};

	void Draw(Point pt) {
		cv::Point2f this_point_ = {pt.x_, pt.y_};
		cv::Point2f pt_pixel_ = this_point_ * this->scaling_ + this->shift_;
		cv::circle(this->canvas_, pt_pixel_, g_GlobalVars.visualize_point_size, COLOR_RED, -1);
	};

	void Draw(Line line) {
		cv::Point2f term_point_1_;
		cv::Point2f term_point_2_;
		term_point_1_.x = line.center_.x_ + g_GlobalVars.convention_infinity * cosf32(line.theta_);
		term_point_1_.y = line.center_.y_ + g_GlobalVars.convention_infinity * sinf32(line.theta_);
		term_point_2_.x = line.center_.x_ - g_GlobalVars.convention_infinity * cosf32(line.theta_);
		term_point_2_.y = line.center_.y_ - g_GlobalVars.convention_infinity * sinf32(line.theta_);
		// LOG(INFO) << term_point_1_.x << " " << term_point_1_.y << " " << term_point_2_.x << " " << term_point_2_.y;
		term_point_1_ = term_point_1_ * this->scaling_ + this->shift_;
		term_point_2_ = term_point_2_ * this->scaling_ + this->shift_;
		LinePositivate(term_point_1_, term_point_2_);
		cv::line(this->canvas_, term_point_1_, term_point_2_, COLOR_GREEN, g_GlobalVars.visualize_line_width);
	};

	void Draw(HalfLine half_line) {
		cv::Point2f term_point_1_ = {half_line.center_.x_, half_line.center_.y_};
		cv::Point2f term_point_2_;
		term_point_2_.x = half_line.center_.x_ + g_GlobalVars.convention_infinity * cosf32(half_line.theta_);
		term_point_2_.y = half_line.center_.y_ + g_GlobalVars.convention_infinity * sinf32(half_line.theta_);
		term_point_1_ = term_point_1_ * this->scaling_ + this->shift_;
		term_point_2_ = term_point_2_ * this->scaling_ + this->shift_;
		LinePositivate(term_point_1_, term_point_2_);
		cv::line(this->canvas_, term_point_1_, term_point_2_, COLOR_YELLOW, g_GlobalVars.visualize_line_width);
	};

	void Draw(Segment segment) {
		cv::Point2f term_point_1_ = {segment.terminal_vertex_1_.x_, segment.terminal_vertex_1_.y_};
		cv::Point2f term_point_2_ = {segment.terminal_vertex_2_.x_, segment.terminal_vertex_2_.y_};
		term_point_1_ = term_point_1_ * this->scaling_ + this->shift_;
		term_point_2_ = term_point_2_ * this->scaling_ + this->shift_;
		LinePositivate(term_point_1_, term_point_2_);
		cv::line(this->canvas_, term_point_1_, term_point_2_, COLOR_BLUE, g_GlobalVars.visualize_line_width);
	};

	void Visualize(std::string window_name) {
		cv::namedWindow(window_name, cv::WINDOW_KEEPRATIO);
		cv::resizeWindow(window_name, cv::Size(g_GlobalVars.visualize_window_width, g_GlobalVars.visualize_window_height));
		cv::Point mouse_location_(-g_GlobalVars.convention_error_code, -g_GlobalVars.convention_error_code);
		cv::setMouseCallback(window_name, MouseLocator, &mouse_location_);
		cv::Point show_location_(5, 15);
		std::string s_;
		cv::Vec3b bgr_;
		int b_, g_, r_;
		cv::Mat visual_;
		while (cv::waitKey(30) != 'q') {
			// LOG(INFO) << mouse_location_.x << " " << mouse_location_.y;
			visual_ = this->canvas_.clone();
			cv::flip(visual_, visual_, 0);
			if (mouse_location_.x >= 0 && mouse_location_.y >= 0 && mouse_location_.x < visual_.cols &&
					mouse_location_.y < visual_.rows) {
				bgr_ = visual_.at<cv::Vec3b>(mouse_location_.y, mouse_location_.x);
				b_ = int(bgr_[0]);
				g_ = int(bgr_[1]);
				r_ = int(bgr_[2]);
				s_ = "X:" + std::to_string(mouse_location_.x) + " Y:" + std::to_string(mouse_location_.y) + " RGB:(" +
						 std::to_string(r_) + "," + std::to_string(g_) + "," + std::to_string(b_) + ")";
			} else if (mouse_location_.x < 0 && mouse_location_.y < 0 && mouse_location_.x > -visual_.cols + 1 &&
								 mouse_location_.y > -visual_.rows + 1) {
				cv::Point2f geo_coord_;
				geo_coord_.x = (float(-mouse_location_.x) - this->shift_x_) / this->scaling_;
				geo_coord_.y = (float(visual_.rows - 1 + mouse_location_.y) - this->shift_y_) / this->scaling_;
				s_ = "(" + StringFloatPrecision(std::to_string(geo_coord_.x), g_GlobalVars.visualize_precision) + "," +
						 StringFloatPrecision(std::to_string(geo_coord_.y), g_GlobalVars.visualize_precision) + ")";
			} else {
				s_ = "";
				b_ = g_ = r_ = 0;
			}
			cv::putText(visual_, s_, show_location_, CV_FONT_HERSHEY_TRIPLEX, g_GlobalVars.visualize_font_scale,
									COLOR_NOTATION, g_GlobalVars.visualize_font_thickness);
			cv::imshow(window_name, visual_);
		}
	};

 private:
	float scale_x_ = 0.0, scale_y_ = 0.0;
	float max_x_ = -g_GlobalVars.convention_infinity, min_x_ = g_GlobalVars.convention_infinity,
				max_y_ = -g_GlobalVars.convention_infinity, min_y_ = g_GlobalVars.convention_infinity;

	// Euclidean coord * scaling + shift = pixel_coord
	float scaling_ = 1.0;
	float shift_x_ = 0.0, shift_y_ = 0.0;
	cv::Point2f shift_ = {shift_x_, shift_y_};
	int pixel_width = 0, pixel_height = 0;

	static void LinePositivate(cv::Point2f &pt_1, cv::Point2f &pt_2) {
		if (pt_1.x < 0.) {
			if (pt_2.x == pt_1.x) {
				LOG(ERROR) << "elements not in canvas!";
			} else {
				pt_1.y += (pt_2.y - pt_1.y) / (pt_2.x - pt_1.x) * (-pt_1.x);
				pt_1.x = 0.0;
			}
		}
		if (pt_1.y < 0.0) {
			if (pt_2.y == pt_1.y) {
				LOG(ERROR) << "elements not in canvas!";
			} else {
				pt_1.x += (pt_2.x - pt_1.x) / (pt_2.y - pt_1.y) * (-pt_1.y);
				pt_1.y = 0.0;
			}
		}
		if (pt_2.x < 0.0) {
			if (pt_2.x == pt_1.x) {
				LOG(ERROR) << "elements not in canvas!";
			} else {
				pt_2.y += (pt_1.y - pt_2.y) / (pt_1.x - pt_2.x) * (-pt_2.x);
				pt_2.x = 0.0;
			}
		}
		if (pt_2.y < 0.0) {
			if (pt_2.y == pt_1.y) {
				LOG(ERROR) << "elements not in canvas!";
			} else {
				pt_2.x += (pt_1.x - pt_2.x) / (pt_1.y - pt_2.y) * (-pt_2.y);
				pt_2.y = 0.0;
			}
		}
	};
};

}	// namespace Euclidean
}	// namespace GeoChain