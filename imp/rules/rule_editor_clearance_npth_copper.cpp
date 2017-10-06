#include "rule_editor_clearance_npth_copper.hpp"
#include "board/rule_clearance_npth_copper.hpp"
#include "widgets/spin_button_dim.hpp"
#include "rule_match_editor.hpp"
#include "core/core.hpp"
#include "widgets/chooser_buttons.hpp"
#include "widgets/parameter_set_editor.hpp"
#include "core/core_board.hpp"

namespace horizon {
	void RuleEditorClearanceNPTHCopper::populate() {
		rule2 = dynamic_cast<RuleClearanceNPTHCopper*>(rule);

		auto editor = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 10));
		editor->set_margin_start(20);
		editor->set_margin_end(20);
		editor->set_margin_bottom(20);

		auto grid = Gtk::manage(new Gtk::Grid());
		grid->set_row_spacing(10);
		grid->set_column_spacing(20);

		{
			auto la = Gtk::manage(new Gtk::Label("Match"));
			la->get_style_context()->add_class("dim-label");
			la->set_halign(Gtk::ALIGN_START);
			grid->attach(*la, 0,0, 1,1);
		}
		{
			auto la = Gtk::manage(new Gtk::Label("Layer"));
			la->get_style_context()->add_class("dim-label");
			la->set_halign(Gtk::ALIGN_START);
			grid->attach(*la, 1,0, 1,1);
		}

		auto match_editor = Gtk::manage(new RuleMatchEditor(&rule2->match, core));
		match_editor->set_orientation(Gtk::ORIENTATION_HORIZONTAL);
		match_editor->signal_updated().connect([this] {s_signal_updated.emit();});
		grid->attach(*match_editor, 0, 1, 1, 1);

		auto *layer_combo = Gtk::manage(new Gtk::ComboBoxText());
		for(const auto &it: core->get_layer_provider()->get_layers()) {
			if(it.second.copper)
				layer_combo->insert(0, std::to_string(it.first), it.second.name + ": " + std::to_string(it.first));
		}
		layer_combo->insert(0, "10000", "Any layer");
		layer_combo->set_active_id(std::to_string(rule2->layer));
		layer_combo->signal_changed().connect([this, layer_combo] {
			rule2->layer = std::stoi(layer_combo->get_active_id());
			s_signal_updated.emit();
		});
		grid->attach(*layer_combo, 1, 1, 1, 1);

		grid->show_all();
		editor->pack_start(*grid, false, false, 0);


		auto box2 = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 10));
		{
			auto la = Gtk::manage(new Gtk::Label("Clearance"));
			la->get_style_context()->add_class("dim-label");
			la->set_xalign(0);
			box2->pack_start(*la, false, false, 0);
		}
		auto sp_clearance = Gtk::manage(new SpinButtonDim());
		sp_clearance->set_range(0, 50_mm);
		sp_clearance->set_value(rule2->clearance);
		sp_clearance->signal_changed().connect([this, sp_clearance]{
			rule2->clearance = sp_clearance->get_value_as_int();
			s_signal_updated.emit();
		});
		box2->pack_start(*sp_clearance, true, true, 0);

		box2->show_all();
		editor->pack_start(*box2, false, false, 0);


		pack_start(*editor, true, true, 0);
		editor->show();
	}
}