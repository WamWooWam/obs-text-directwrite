
#include "obs_text_directwrite.h"

#include <set>

/* ------------------------------------------------------------------------- */

#define S_FONT "font"
#define S_FONT_GROUP "font_group"
#define S_USE_FILE "read_from_file"
#define S_FILE "file"
#define S_TEXT "text"
#define S_TEXT_GROUP "text_group"
#define S_COLOR "color"
#define S_GRADIENT "gradient"
#define S_GRADIENT_DIR "gradient_dir"
#define S_GRADIENT_OPACITY "gradient_opacity"

#define S_VARIABLE "variable"

#define S_GRADIENT_EX "colors"
#define S_GRADIENT_EX_COUNT "gradient_count"
#define S_GRADIENT_EX_COLOR "gradient_color_%d"
#define S_GRADIENT_EX_OFFSET "gradient_offset_%d"
#define S_GRADIENT_EX_DIR "gradient_dir"
#define S_GRADIENT_EX_OPACITY "gradient_opacity"

#define S_ALIGN "align"
#define S_VALIGN "valign"
#define S_OPACITY "opacity"

#define S_BKGROUP "bk_group"
#define S_BKOPACITY "bk_opacity"
#define S_BKCOLOR "bk_color"
#define S_BKOPACITY "bk_opacity"

//#define S_VERTICAL                      "vertical"
#define S_LAYOUT "layout"

#define S_SHADOW "shadow"
#define S_SHADOW_GROUP "shadow_group"
#define S_SHADOW_OFFSET_X "shadow_offset_x"
#define S_SHADOW_OFFSET_Y "shadow_offset_y"
#define S_SHADOW_RADIUS "shadow_radius"
#define S_SHADOW_COLOR "shadow_color"
#define S_SHADOW_OPACITY "shadow_opacity"

#define S_OUTLINE "outline"
#define S_OUTLINE_GROUP "outline_group"
#define S_OUTLINE_SIZE "outline_size"
#define S_OUTLINE_COLOR "outline_color"
#define S_OUTLINE_OPACITY "outline_opacity"

#define S_CHATLOG_MODE "chatlog"
#define S_CHATLOG_LINES "chatlog_lines"

#define S_EXTENTS "extents"
#define S_EXTENTS_CX "extents_cx"
#define S_EXTENTS_CY "extents_cy"

#define S_ALIGN_LEFT "left"
#define S_ALIGN_CENTER "center"
#define S_ALIGN_RIGHT "right"
#define S_ALIGN_JUSTIFIED "justified"

#define S_VALIGN_TOP "top"
#define S_VALIGN_CENTER S_ALIGN_CENTER
#define S_VALIGN_BOTTOM "bottom"

#define S_WRAP_MODE "wrap_mode"
#define S_WRAP_MODE_NONE "none"
#define S_WRAP_MODE_WRAP "wrap"
#define S_WRAP_MODE_WRAP_WHOLE_WORDS "wrap_whole_words"
#define S_WRAP_MODE_WRAP_CHARACTER "wrap_character"

#define S_COLOR_FONTS "color_fonts"

#define S_ANTIALIASING "antialiasing"

#define S_TRANSFORM "transform"
#define S_TRANSFORM_NONE 0
#define S_TRANSFORM_UPPERCASE 1
#define S_TRANSFORM_LOWERCASE 2
#define S_TRANSFORM_STARTCASE 3

#define S_TRIMMING "trimming"
#define S_TRIMMING_NONE 0
#define S_TRIMMING_CHARACTER_ELLIPSIS 0
#define S_TRIMMING_WORD_ELLIPSIS 2

#define S_FONT_WEIGHT "font_weight"
#define S_FONT_WEIGHT_AUTO -1
#define S_FONT_WEIGHT_100 DWRITE_FONT_WEIGHT_THIN
#define S_FONT_WEIGHT_200 DWRITE_FONT_WEIGHT_EXTRA_LIGHT
#define S_FONT_WEIGHT_300 DWRITE_FONT_WEIGHT_LIGHT
#define S_FONT_WEIGHT_350 DWRITE_FONT_WEIGHT_SEMI_LIGHT
#define S_FONT_WEIGHT_400 DWRITE_FONT_WEIGHT_NORMAL
#define S_FONT_WEIGHT_500 DWRITE_FONT_WEIGHT_MEDIUM
#define S_FONT_WEIGHT_600 DWRITE_FONT_WEIGHT_SEMI_BOLD
#define S_FONT_WEIGHT_700 DWRITE_FONT_WEIGHT_BOLD
#define S_FONT_WEIGHT_800 DWRITE_FONT_WEIGHT_EXTRA_BOLD
#define S_FONT_WEIGHT_900 DWRITE_FONT_WEIGHT_BLACK
#define S_FONT_WEIGHT_950 DWRITE_FONT_WEIGHT_EXTRA_BLACK

#define S_FONT_STRETCH "font_stretch"
#define S_FONT_STRETCH_AUTO -1
#define S_FONT_STRETCH_ULTRA_CONDENSED DWRITE_FONT_STRETCH_ULTRA_CONDENSED
#define S_FONT_STRETCH_EXTRA_CONDENSED DWRITE_FONT_STRETCH_EXTRA_CONDENSED
#define S_FONT_STRETCH_CONDENSED DWRITE_FONT_STRETCH_CONDENSED
#define S_FONT_STRETCH_SEMI_CONDENSED DWRITE_FONT_STRETCH_SEMI_CONDENSED
#define S_FONT_STRETCH_NORMAL DWRITE_FONT_STRETCH_NORMAL
#define S_FONT_STRETCH_SEMI_EXPANDED DWRITE_FONT_STRETCH_SEMI_EXPANDED
#define S_FONT_STRETCH_EXPANDED DWRITE_FONT_STRETCH_EXPANDED
#define S_FONT_STRETCH_EXTRA_EXPANDED DWRITE_FONT_STRETCH_EXTRA_EXPANDED
#define S_FONT_STRETCH_ULTRA_EXPANDED DWRITE_FONT_STRETCH_ULTRA_EXPANDED

#define S_LINE_SPACING "line_spacing"
#define S_LINE_SPACING_DEFAULT DWRITE_LINE_SPACING_METHOD_DEFAULT
#define S_LINE_SPACING_UNIFORM DWRITE_LINE_SPACING_METHOD_UNIFORM

#define S_LINE_SPACING_RATIO "line_spacing_ratio"

#define S_FONT_FACE "face"
#define S_FONT_SIZE "size"
#define S_FONT_FLAGS "flags"

#define S_FONT_STYLE "style"
#define S_FONT_STYLE_NONE "none"
#define S_FONT_STYLE_ITALIC "italic"
#define S_FONT_STYLE_OBLIQUE "oblique"

#define S_FONT_UNDERLINE "underline"
#define S_FONT_STRIKETHROUGH "strikethrough"

// Font features settings group key
#define S_FONT_FEATURES "font_features"

#define S_OPTICAL_SIZE "variable_prop::opsz"
#define S_AUTO_OPTICAL_SIZE "variable_prop::opsz::auto"

// Individual font feature setting keys
#define S_FONT_FEATURE_ALTERNATIVE_FRACTIONS "font_feature_alternative_fractions"
#define S_FONT_FEATURE_PETITE_CAPITALS_FROM_CAPITALS "font_feature_petite_capitals_from_capitals"
#define S_FONT_FEATURE_SMALL_CAPITALS_FROM_CAPITALS "font_feature_small_capitals_from_capitals"
#define S_FONT_FEATURE_CONTEXTUAL_ALTERNATES "font_feature_contextual_alternates"
#define S_FONT_FEATURE_CASE_SENSITIVE_FORMS "font_feature_case_sensitive_forms"
#define S_FONT_FEATURE_GLYPH_COMPOSITION_DECOMPOSITION "font_feature_glyph_composition_decomposition"
#define S_FONT_FEATURE_CONTEXTUAL_LIGATURES "font_feature_contextual_ligatures"
#define S_FONT_FEATURE_CAPITAL_SPACING "font_feature_capital_spacing"
#define S_FONT_FEATURE_CONTEXTUAL_SWASH "font_feature_contextual_swash"
#define S_FONT_FEATURE_CURSIVE_POSITIONING "font_feature_cursive_positioning"
#define S_FONT_FEATURE_DEFAULT "font_feature_default"
#define S_FONT_FEATURE_DISCRETIONARY_LIGATURES "font_feature_discretionary_ligatures"
#define S_FONT_FEATURE_EXPERT_FORMS "font_feature_expert_forms"
#define S_FONT_FEATURE_FRACTIONS "font_feature_fractions"
#define S_FONT_FEATURE_FULL_WIDTH "font_feature_full_width"
#define S_FONT_FEATURE_HALF_FORMS "font_feature_half_forms"
#define S_FONT_FEATURE_HALANT_FORMS "font_feature_halant_forms"
#define S_FONT_FEATURE_ALTERNATE_HALF_WIDTH "font_feature_alternate_half_width"
#define S_FONT_FEATURE_HISTORICAL_FORMS "font_feature_historical_forms"
#define S_FONT_FEATURE_HORIZONTAL_KANA_ALTERNATES "font_feature_horizontal_kana_alternates"
#define S_FONT_FEATURE_HISTORICAL_LIGATURES "font_feature_historical_ligatures"
#define S_FONT_FEATURE_HALF_WIDTH "font_feature_half_width"
#define S_FONT_FEATURE_HOJO_KANJI_FORMS "font_feature_hojo_kanji_forms"
#define S_FONT_FEATURE_JIS04_FORMS "font_feature_jis04_forms"
#define S_FONT_FEATURE_JIS78_FORMS "font_feature_jis78_forms"
#define S_FONT_FEATURE_JIS83_FORMS "font_feature_jis83_forms"
#define S_FONT_FEATURE_JIS90_FORMS "font_feature_jis90_forms"
#define S_FONT_FEATURE_KERNING "font_feature_kerning"
#define S_FONT_FEATURE_STANDARD_LIGATURES "font_feature_standard_ligatures"
#define S_FONT_FEATURE_LINING_FIGURES "font_feature_lining_figures"
#define S_FONT_FEATURE_LOCALIZED_FORMS "font_feature_localized_forms"
#define S_FONT_FEATURE_MARK_POSITIONING "font_feature_mark_positioning"
#define S_FONT_FEATURE_MATHEMATICAL_GREEK "font_feature_mathematical_greek"
#define S_FONT_FEATURE_MARK_TO_MARK_POSITIONING "font_feature_mark_to_mark_positioning"
#define S_FONT_FEATURE_ALTERNATE_ANNOTATION_FORMS "font_feature_alternate_annotation_forms"
#define S_FONT_FEATURE_NLC_KANJI_FORMS "font_feature_nlc_kanji_forms"
#define S_FONT_FEATURE_OLD_STYLE_FIGURES "font_feature_old_style_figures"
#define S_FONT_FEATURE_ORDINALS "font_feature_ordinals"
#define S_FONT_FEATURE_PROPORTIONAL_ALTERNATE_WIDTH "font_feature_proportional_alternate_width"
#define S_FONT_FEATURE_PETITE_CAPITALS "font_feature_petite_capitals"
#define S_FONT_FEATURE_PROPORTIONAL_FIGURES "font_feature_proportional_figures"
#define S_FONT_FEATURE_PROPORTIONAL_WIDTHS "font_feature_proportional_widths"
#define S_FONT_FEATURE_QUARTER_WIDTHS "font_feature_quarter_widths"
#define S_FONT_FEATURE_REQUIRED_LIGATURES "font_feature_required_ligatures"
#define S_FONT_FEATURE_RUBY_NOTATION_FORMS "font_feature_ruby_notation_forms"
#define S_FONT_FEATURE_STYLISTIC_ALTERNATES "font_feature_stylistic_alternates"
#define S_FONT_FEATURE_SCIENTIFIC_INFERIORS "font_feature_scientific_inferiors"
#define S_FONT_FEATURE_SMALL_CAPITALS "font_feature_small_capitals"
#define S_FONT_FEATURE_SIMPLIFIED_FORMS "font_feature_simplified_forms"
#define S_FONT_FEATURE_STYLISTIC_SET_1 "font_feature_stylistic_set_1"
#define S_FONT_FEATURE_STYLISTIC_SET_2 "font_feature_stylistic_set_2"
#define S_FONT_FEATURE_STYLISTIC_SET_3 "font_feature_stylistic_set_3"
#define S_FONT_FEATURE_STYLISTIC_SET_4 "font_feature_stylistic_set_4"
#define S_FONT_FEATURE_STYLISTIC_SET_5 "font_feature_stylistic_set_5"
#define S_FONT_FEATURE_STYLISTIC_SET_6 "font_feature_stylistic_set_6"
#define S_FONT_FEATURE_STYLISTIC_SET_7 "font_feature_stylistic_set_7"
#define S_FONT_FEATURE_STYLISTIC_SET_8 "font_feature_stylistic_set_8"
#define S_FONT_FEATURE_STYLISTIC_SET_9 "font_feature_stylistic_set_9"
#define S_FONT_FEATURE_STYLISTIC_SET_10 "font_feature_stylistic_set_10"
#define S_FONT_FEATURE_STYLISTIC_SET_11 "font_feature_stylistic_set_11"
#define S_FONT_FEATURE_STYLISTIC_SET_12 "font_feature_stylistic_set_12"
#define S_FONT_FEATURE_STYLISTIC_SET_13 "font_feature_stylistic_set_13"
#define S_FONT_FEATURE_STYLISTIC_SET_14 "font_feature_stylistic_set_14"
#define S_FONT_FEATURE_STYLISTIC_SET_15 "font_feature_stylistic_set_15"
#define S_FONT_FEATURE_STYLISTIC_SET_16 "font_feature_stylistic_set_16"
#define S_FONT_FEATURE_STYLISTIC_SET_17 "font_feature_stylistic_set_17"
#define S_FONT_FEATURE_STYLISTIC_SET_18 "font_feature_stylistic_set_18"
#define S_FONT_FEATURE_STYLISTIC_SET_19 "font_feature_stylistic_set_19"
#define S_FONT_FEATURE_STYLISTIC_SET_20 "font_feature_stylistic_set_20"
#define S_FONT_FEATURE_SUBSCRIPT "font_feature_subscript"
#define S_FONT_FEATURE_SUPERSCRIPT "font_feature_superscript"
#define S_FONT_FEATURE_SWASH "font_feature_swash"
#define S_FONT_FEATURE_TITLING "font_feature_titling"
#define S_FONT_FEATURE_TRADITIONAL_NAME_FORMS "font_feature_traditional_name_forms"
#define S_FONT_FEATURE_TABULAR_FIGURES "font_feature_tabular_figures"
#define S_FONT_FEATURE_TRADITIONAL_FORMS "font_feature_traditional_forms"
#define S_FONT_FEATURE_THIRD_WIDTHS "font_feature_third_widths"
#define S_FONT_FEATURE_UNICASE "font_feature_unicase"
#define S_FONT_FEATURE_VERTICAL_WRITING "font_feature_vertical_writing"
#define S_FONT_FEATURE_VERTICAL_ALTERNATES_AND_ROTATION "font_feature_vertical_alternates_and_rotation"
#define S_FONT_FEATURE_SLASHED_ZERO "font_feature_slashed_zero"

#define S_ADVANCED "advanced"

#define S_HAS_VARIABLES "has_variables"

#define T_(v) obs_module_text(v)
#define T_FONT				T_("Font")
#define T_FONT_FACE			T_("FontFamily")
#define T_USE_FILE			T_("ReadFromFile")
#define T_FILE				T_("TextFile")
#define T_TEXT				T_("Text")
#define T_COLOR				T_("Color")
#define T_COLORS			T_("Colors")
#define T_OFFSET			T_("Offset")
#define T_GRADIENT			T_("Gradient")
#define T_GRADIENT_DIR		T_("Gradient.Direction")
#define T_GRADIENT_OPACITY	T_("Gradient.Opacity")
#define T_ALIGN				T_("Alignment")
#define T_VALIGN			T_("VerticalAlignment")
#define T_OPACITY			T_("Opacity")

#define T_BKGROUP			T_("Background")
#define T_BKCOLOR			T_("BkColor")
#define T_BKOPACITY			T_("BkOpacity")

#define T_VARIABLE			T_("FontPropsVariable")

#define T_LAYOUT			T_("Layout")

#define T_SHADOW			T_("Shadow")
#define T_SHADOW_OFFSET_X	T_("Shadow.OffsetX")
#define T_SHADOW_OFFSET_Y	T_("Shadow.OffsetY")
#define T_SHADOW_RADIUS		T_("Shadow.Radius")
#define T_SHADOW_COLOR		T_("Shadow.Color")
#define T_SHADOW_OPACITY	T_("Shadow.Opacity")

#define T_OUTLINE			T_("Outline")
#define T_OUTLINE_SIZE		T_("Outline.Size")
#define T_OUTLINE_COLOR		T_("Outline.Color")
#define T_OUTLINE_OPACITY	T_("Outline.Opacity")

#define T_CHATLOG_MODE		T_("ChatlogMode")
#define T_CHATLOG_LINES		T_("ChatlogMode.Lines")

#define T_EXTENTS			T_("UseCustomExtents")
#define T_EXTENTS_CX		T_("Width")
#define T_EXTENTS_CY		T_("Height")

#define T_FILTER_TEXT_FILES T_("Filter.TextFiles")
#define T_FILTER_ALL_FILES	T_("Filter.AllFiles")

#define T_ALIGN_LEFT		T_("Alignment.Left")
#define T_ALIGN_CENTER		T_("Alignment.Center")
#define T_ALIGN_RIGHT		T_("Alignment.Right")
#define T_ALIGN_JUSTIFIED	T_("Alignment.Justified")

#define T_VALIGN_TOP		T_("VerticalAlignment.Top")
#define T_VALIGN_CENTER		T_ALIGN_CENTER
#define T_VALIGN_BOTTOM		T_("VerticalAlignment.Bottom")

#define T_WRAP_MODE						T_("WordWrap")
#define T_WRAP_MODE_NONE				T_("WordWrap.None")
#define T_WRAP_MODE_WRAP				T_("WordWrap.Wrap")
#define T_WRAP_MODE_WRAP_WHOLE_WORDS	T_("WordWrap.WrapWholeWords")
#define T_WRAP_MODE_WRAP_CHARACTER		T_("WordWrap.WrapCharacter")

#define T_COLOR_FONTS		T_("ColorFonts")
#define T_ANTIALIASING		T_("Antialiasing")

#define T_TRANSFORM				T_("Transform")
#define T_TRANSFORM_NONE		T_("Transform.None")
#define T_TRANSFORM_UPPERCASE	T_("Transform.Uppercase")
#define T_TRANSFORM_LOWERCASE	T_("Transform.Lowercase")
#define T_TRANSFORM_STARTCASE	T_("Transform.Startcase")

#define T_TRIMMING						T_("TextTrimming")
#define T_TRIMMING_NONE					T_("TextTrimming.None")
#define T_TRIMMING_CHARACTER_ELLIPSIS	T_("TextTrimming.CharacterEllipsis")
#define T_TRIMMING_WORD_ELLIPSIS		T_("TextTrimming.WordEllipsis")

#define T_FONT_SIZE						T_("Font.Size")

#define T_FONT_WEIGHT		T_("FontWeight")
#define T_FONT_WEIGHT_AUTO	T_("FontWeight.Auto")
#define T_FONT_WEIGHT_100	T_("FontWeight.100")
#define T_FONT_WEIGHT_200	T_("FontWeight.200")
#define T_FONT_WEIGHT_300	T_("FontWeight.300")
#define T_FONT_WEIGHT_350	T_("FontWeight.350")
#define T_FONT_WEIGHT_400	T_("FontWeight.400")
#define T_FONT_WEIGHT_500	T_("FontWeight.500")
#define T_FONT_WEIGHT_600	T_("FontWeight.600")
#define T_FONT_WEIGHT_700	T_("FontWeight.700")
#define T_FONT_WEIGHT_800	T_("FontWeight.800")
#define T_FONT_WEIGHT_900	T_("FontWeight.900")
#define T_FONT_WEIGHT_950	T_("FontWeight.950")

#define T_FONT_STRETCH					T_("FontStretch")
#define T_FONT_STRETCH_AUTO				T_("FontStretch.Auto")
#define T_FONT_STRETCH_ULTRA_CONDENSED	T_("FontStretch.UltraCondensed")
#define T_FONT_STRETCH_EXTRA_CONDENSED	T_("FontStretch.ExtraCondensed")
#define T_FONT_STRETCH_CONDENSED		T_("FontStretch.Condensed")
#define T_FONT_STRETCH_SEMI_CONDENSED	T_("FontStretch.SemiCondensed")
#define T_FONT_STRETCH_NORMAL			T_("FontStretch.Normal")
#define T_FONT_STRETCH_SEMI_EXPANDED	T_("FontStretch.SemiExpanded")
#define T_FONT_STRETCH_EXPANDED			T_("FontStretch.Expanded")
#define T_FONT_STRETCH_EXTRA_EXPANDED	T_("FontStretch.ExtraExpanded")
#define T_FONT_STRETCH_ULTRA_EXPANDED	T_("FontStretch.UltraExpanded")

#define T_FONT_STYLE			T_("FontStyle")
#define T_FONT_STYLE_NONE		T_("FontStyle.None")
#define T_FONT_STYLE_ITALIC		T_("FontStyle.Italic")
#define T_FONT_STYLE_OBLIQUE	T_("FontStyle.Oblique")

#define T_LINE_SPACING			T_("LineSpacing")
#define T_LINE_SPACING_DEFAULT	T_("LineSpacing.Default")
#define T_LINE_SPACING_UNIFORM	T_("LineSpacing.Uniform")
#define T_LINE_SPACING_RATIO	T_("LineSpacing.Ratio")

#define T_FONT_UNDERLINE		T_("Font.Underline")
#define T_FONT_STRIKETHROUGH	T_("Font.Strikethrough")

#define T_AUTO_OPTICAL_SIZE		T_("Axis.opsz.Auto")

// Font features translation group key
#define T_FONT_FEATURES T_("FontFeatures")

// Individual font feature translation keys
#define T_FONT_FEATURE_ALTERNATIVE_FRACTIONS T_("FontFeature.AlternativeFractions")
#define T_FONT_FEATURE_PETITE_CAPITALS_FROM_CAPITALS T_("FontFeature.PetiteCapitalsFromCapitals")
#define T_FONT_FEATURE_SMALL_CAPITALS_FROM_CAPITALS T_("FontFeature.SmallCapitalsFromCapitals")
#define T_FONT_FEATURE_CONTEXTUAL_ALTERNATES T_("FontFeature.ContextualAlternates")
#define T_FONT_FEATURE_CASE_SENSITIVE_FORMS T_("FontFeature.CaseSensitiveForms")
#define T_FONT_FEATURE_GLYPH_COMPOSITION_DECOMPOSITION T_("FontFeature.GlyphCompositionDecomposition")
#define T_FONT_FEATURE_CONTEXTUAL_LIGATURES T_("FontFeature.ContextualLigatures")
#define T_FONT_FEATURE_CAPITAL_SPACING T_("FontFeature.CapitalSpacing")
#define T_FONT_FEATURE_CONTEXTUAL_SWASH T_("FontFeature.ContextualSwash")
#define T_FONT_FEATURE_CURSIVE_POSITIONING T_("FontFeature.CursivePositioning")
#define T_FONT_FEATURE_DEFAULT T_("FontFeature.Default")
#define T_FONT_FEATURE_DISCRETIONARY_LIGATURES T_("FontFeature.DiscretionaryLigatures")
#define T_FONT_FEATURE_EXPERT_FORMS T_("FontFeature.ExpertForms")
#define T_FONT_FEATURE_FRACTIONS T_("FontFeature.Fractions")
#define T_FONT_FEATURE_FULL_WIDTH T_("FontFeature.FullWidth")
#define T_FONT_FEATURE_HALF_FORMS T_("FontFeature.HalfForms")
#define T_FONT_FEATURE_HALANT_FORMS T_("FontFeature.HalantForms")
#define T_FONT_FEATURE_ALTERNATE_HALF_WIDTH T_("FontFeature.AlternateHalfWidth")
#define T_FONT_FEATURE_HISTORICAL_FORMS T_("FontFeature.HistoricalForms")
#define T_FONT_FEATURE_HORIZONTAL_KANA_ALTERNATES T_("FontFeature.HorizontalKanaAlternates")
#define T_FONT_FEATURE_HISTORICAL_LIGATURES T_("FontFeature.HistoricalLigatures")
#define T_FONT_FEATURE_HALF_WIDTH T_("FontFeature.HalfWidth")
#define T_FONT_FEATURE_HOJO_KANJI_FORMS T_("FontFeature.HojoKanjiForms")
#define T_FONT_FEATURE_JIS04_FORMS T_("FontFeature.JIS04Forms")
#define T_FONT_FEATURE_JIS78_FORMS T_("FontFeature.JIS78Forms")
#define T_FONT_FEATURE_JIS83_FORMS T_("FontFeature.JIS83Forms")
#define T_FONT_FEATURE_JIS90_FORMS T_("FontFeature.JIS90Forms")
#define T_FONT_FEATURE_KERNING T_("FontFeature.Kerning")
#define T_FONT_FEATURE_STANDARD_LIGATURES T_("FontFeature.StandardLigatures")
#define T_FONT_FEATURE_LINING_FIGURES T_("FontFeature.LiningFigures")
#define T_FONT_FEATURE_LOCALIZED_FORMS T_("FontFeature.LocalizedForms")
#define T_FONT_FEATURE_MARK_POSITIONING T_("FontFeature.MarkPositioning")
#define T_FONT_FEATURE_MATHEMATICAL_GREEK T_("FontFeature.MathematicalGreek")
#define T_FONT_FEATURE_MARK_TO_MARK_POSITIONING T_("FontFeature.MarkToMarkPositioning")
#define T_FONT_FEATURE_ALTERNATE_ANNOTATION_FORMS T_("FontFeature.AlternateAnnotationForms")
#define T_FONT_FEATURE_NLC_KANJI_FORMS T_("FontFeature.NLCKanjiForms")
#define T_FONT_FEATURE_OLD_STYLE_FIGURES T_("FontFeature.OldStyleFigures")
#define T_FONT_FEATURE_ORDINALS T_("FontFeature.Ordinals")
#define T_FONT_FEATURE_PROPORTIONAL_ALTERNATE_WIDTH T_("FontFeature.ProportionalAlternateWidth")
#define T_FONT_FEATURE_PETITE_CAPITALS T_("FontFeature.PetiteCapitals")
#define T_FONT_FEATURE_PROPORTIONAL_FIGURES T_("FontFeature.ProportionalFigures")
#define T_FONT_FEATURE_PROPORTIONAL_WIDTHS T_("FontFeature.ProportionalWidths")
#define T_FONT_FEATURE_QUARTER_WIDTHS T_("FontFeature.QuarterWidths")
#define T_FONT_FEATURE_REQUIRED_LIGATURES T_("FontFeature.RequiredLigatures")
#define T_FONT_FEATURE_RUBY_NOTATION_FORMS T_("FontFeature.RubyNotationForms")
#define T_FONT_FEATURE_STYLISTIC_ALTERNATES T_("FontFeature.StylisticAlternates")
#define T_FONT_FEATURE_SCIENTIFIC_INFERIORS T_("FontFeature.ScientificInferiors")
#define T_FONT_FEATURE_SMALL_CAPITALS T_("FontFeature.SmallCapitals")
#define T_FONT_FEATURE_SIMPLIFIED_FORMS T_("FontFeature.SimplifiedForms")
#define T_FONT_FEATURE_STYLISTIC_SET_1 T_("FontFeature.StylisticSet1")
#define T_FONT_FEATURE_STYLISTIC_SET_2 T_("FontFeature.StylisticSet2")
#define T_FONT_FEATURE_STYLISTIC_SET_3 T_("FontFeature.StylisticSet3")
#define T_FONT_FEATURE_STYLISTIC_SET_4 T_("FontFeature.StylisticSet4")
#define T_FONT_FEATURE_STYLISTIC_SET_5 T_("FontFeature.StylisticSet5")
#define T_FONT_FEATURE_STYLISTIC_SET_6 T_("FontFeature.StylisticSet6")
#define T_FONT_FEATURE_STYLISTIC_SET_7 T_("FontFeature.StylisticSet7")
#define T_FONT_FEATURE_STYLISTIC_SET_8 T_("FontFeature.StylisticSet8")
#define T_FONT_FEATURE_STYLISTIC_SET_9 T_("FontFeature.StylisticSet9")
#define T_FONT_FEATURE_STYLISTIC_SET_10 T_("FontFeature.StylisticSet10")
#define T_FONT_FEATURE_STYLISTIC_SET_11 T_("FontFeature.StylisticSet11")
#define T_FONT_FEATURE_STYLISTIC_SET_12 T_("FontFeature.StylisticSet12")
#define T_FONT_FEATURE_STYLISTIC_SET_13 T_("FontFeature.StylisticSet13")
#define T_FONT_FEATURE_STYLISTIC_SET_14 T_("FontFeature.StylisticSet14")
#define T_FONT_FEATURE_STYLISTIC_SET_15 T_("FontFeature.StylisticSet15")
#define T_FONT_FEATURE_STYLISTIC_SET_16 T_("FontFeature.StylisticSet16")
#define T_FONT_FEATURE_STYLISTIC_SET_17 T_("FontFeature.StylisticSet17")
#define T_FONT_FEATURE_STYLISTIC_SET_18 T_("FontFeature.StylisticSet18")
#define T_FONT_FEATURE_STYLISTIC_SET_19 T_("FontFeature.StylisticSet19")
#define T_FONT_FEATURE_STYLISTIC_SET_20 T_("FontFeature.StylisticSet20")
#define T_FONT_FEATURE_SUBSCRIPT T_("FontFeature.Subscript")
#define T_FONT_FEATURE_SUPERSCRIPT T_("FontFeature.Superscript")
#define T_FONT_FEATURE_SWASH T_("FontFeature.Swash")
#define T_FONT_FEATURE_TITLING T_("FontFeature.Titling")
#define T_FONT_FEATURE_TRADITIONAL_NAME_FORMS T_("FontFeature.TraditionalNameForms")
#define T_FONT_FEATURE_TABULAR_FIGURES T_("FontFeature.TabularFigures")
#define T_FONT_FEATURE_TRADITIONAL_FORMS T_("FontFeature.TraditionalForms")
#define T_FONT_FEATURE_THIRD_WIDTHS T_("FontFeature.ThirdWidths")
#define T_FONT_FEATURE_UNICASE T_("FontFeature.Unicase")
#define T_FONT_FEATURE_VERTICAL_WRITING T_("FontFeature.VerticalWriting")
#define T_FONT_FEATURE_VERTICAL_ALTERNATES_AND_ROTATION T_("FontFeature.VerticalAlternatesAndRotation")
#define T_FONT_FEATURE_SLASHED_ZERO T_("FontFeature.SlashedZero")

#define T_HTML T_("Html")

#define T_ADVANCED T_("Advanced")

static const char gradient_stop_color_names[MAX_GRADIENT_STOPS][32]{
	"gradient_color_0",  "gradient_color_1",  "gradient_color_2",  "gradient_color_3",
	"gradient_color_4",  "gradient_color_5",  "gradient_color_6",  "gradient_color_7",
	"gradient_color_8",  "gradient_color_9",  "gradient_color_10", "gradient_color_11",
	"gradient_color_12", "gradient_color_13", "gradient_color_14", "gradient_color_15",
};

static const float stretchToWidth[] = {
	0.0f,    // undefined (0)
	50.0f,   // ultra-condensed
	62.5f,   // extra-condensed
	75.0f,   // condensed
	87.5f,   // semi-condensed
	100.0f,  // normal
	112.5f,  // semi-expanded
	125.0f,  // expanded
	150.0f,  // extra-expanded
	200.0f,  // ultra-expanded
};

static const std::set<font_feature_tag, std::less<>> font_feature_set = {
	{"font_feature_alternative_fractions", DWRITE_FONT_FEATURE_TAG_ALTERNATIVE_FRACTIONS},
	{"font_feature_petite_capitals_from_capitals", DWRITE_FONT_FEATURE_TAG_PETITE_CAPITALS_FROM_CAPITALS},
	{"font_feature_small_capitals_from_capitals", DWRITE_FONT_FEATURE_TAG_SMALL_CAPITALS_FROM_CAPITALS},
	{"font_feature_contextual_alternates", DWRITE_FONT_FEATURE_TAG_CONTEXTUAL_ALTERNATES},
	{"font_feature_case_sensitive_forms", DWRITE_FONT_FEATURE_TAG_CASE_SENSITIVE_FORMS},
	{"font_feature_glyph_composition_decomposition", DWRITE_FONT_FEATURE_TAG_GLYPH_COMPOSITION_DECOMPOSITION},
	{"font_feature_contextual_ligatures", DWRITE_FONT_FEATURE_TAG_CONTEXTUAL_LIGATURES},
	{"font_feature_capital_spacing", DWRITE_FONT_FEATURE_TAG_CAPITAL_SPACING},
	{"font_feature_contextual_swash", DWRITE_FONT_FEATURE_TAG_CONTEXTUAL_SWASH},
	{"font_feature_cursive_positioning", DWRITE_FONT_FEATURE_TAG_CURSIVE_POSITIONING},
	{"font_feature_default", DWRITE_FONT_FEATURE_TAG_DEFAULT},
	{"font_feature_discretionary_ligatures", DWRITE_FONT_FEATURE_TAG_DISCRETIONARY_LIGATURES},
	{"font_feature_expert_forms", DWRITE_FONT_FEATURE_TAG_EXPERT_FORMS},
	{"font_feature_fractions", DWRITE_FONT_FEATURE_TAG_FRACTIONS},
	{"font_feature_full_width", DWRITE_FONT_FEATURE_TAG_FULL_WIDTH},
	{"font_feature_half_forms", DWRITE_FONT_FEATURE_TAG_HALF_FORMS},
	{"font_feature_halant_forms", DWRITE_FONT_FEATURE_TAG_HALANT_FORMS},
	{"font_feature_alternate_half_width", DWRITE_FONT_FEATURE_TAG_ALTERNATE_HALF_WIDTH},
	{"font_feature_historical_forms", DWRITE_FONT_FEATURE_TAG_HISTORICAL_FORMS},
	{"font_feature_horizontal_kana_alternates", DWRITE_FONT_FEATURE_TAG_HORIZONTAL_KANA_ALTERNATES},
	{"font_feature_historical_ligatures", DWRITE_FONT_FEATURE_TAG_HISTORICAL_LIGATURES},
	{"font_feature_half_width", DWRITE_FONT_FEATURE_TAG_HALF_WIDTH},
	{"font_feature_hojo_kanji_forms", DWRITE_FONT_FEATURE_TAG_HOJO_KANJI_FORMS},
	{"font_feature_jis04_forms", DWRITE_FONT_FEATURE_TAG_JIS04_FORMS},
	{"font_feature_jis78_forms", DWRITE_FONT_FEATURE_TAG_JIS78_FORMS},
	{"font_feature_jis83_forms", DWRITE_FONT_FEATURE_TAG_JIS83_FORMS},
	{"font_feature_jis90_forms", DWRITE_FONT_FEATURE_TAG_JIS90_FORMS},
	{"font_feature_kerning", DWRITE_FONT_FEATURE_TAG_KERNING},
	{"font_feature_standard_ligatures", DWRITE_FONT_FEATURE_TAG_STANDARD_LIGATURES},
	{"font_feature_lining_figures", DWRITE_FONT_FEATURE_TAG_LINING_FIGURES},
	{"font_feature_localized_forms", DWRITE_FONT_FEATURE_TAG_LOCALIZED_FORMS},
	{"font_feature_mark_positioning", DWRITE_FONT_FEATURE_TAG_MARK_POSITIONING},
	{"font_feature_mathematical_greek", DWRITE_FONT_FEATURE_TAG_MATHEMATICAL_GREEK},
	{"font_feature_mark_to_mark_positioning", DWRITE_FONT_FEATURE_TAG_MARK_TO_MARK_POSITIONING},
	{"font_feature_alternate_annotation_forms", DWRITE_FONT_FEATURE_TAG_ALTERNATE_ANNOTATION_FORMS},
	{"font_feature_nlc_kanji_forms", DWRITE_FONT_FEATURE_TAG_NLC_KANJI_FORMS},
	{"font_feature_old_style_figures", DWRITE_FONT_FEATURE_TAG_OLD_STYLE_FIGURES},
	{"font_feature_ordinals", DWRITE_FONT_FEATURE_TAG_ORDINALS},
	{"font_feature_proportional_alternate_width", DWRITE_FONT_FEATURE_TAG_PROPORTIONAL_ALTERNATE_WIDTH},
	{"font_feature_petite_capitals", DWRITE_FONT_FEATURE_TAG_PETITE_CAPITALS},
	{"font_feature_proportional_figures", DWRITE_FONT_FEATURE_TAG_PROPORTIONAL_FIGURES},
	{"font_feature_proportional_widths", DWRITE_FONT_FEATURE_TAG_PROPORTIONAL_WIDTHS},
	{"font_feature_quarter_widths", DWRITE_FONT_FEATURE_TAG_QUARTER_WIDTHS},
	{"font_feature_required_ligatures", DWRITE_FONT_FEATURE_TAG_REQUIRED_LIGATURES},
	{"font_feature_ruby_notation_forms", DWRITE_FONT_FEATURE_TAG_RUBY_NOTATION_FORMS},
	{"font_feature_stylistic_alternates", DWRITE_FONT_FEATURE_TAG_STYLISTIC_ALTERNATES},
	{"font_feature_scientific_inferiors", DWRITE_FONT_FEATURE_TAG_SCIENTIFIC_INFERIORS},
	{"font_feature_small_capitals", DWRITE_FONT_FEATURE_TAG_SMALL_CAPITALS},
	{"font_feature_simplified_forms", DWRITE_FONT_FEATURE_TAG_SIMPLIFIED_FORMS},
	{"font_feature_stylistic_set_1", DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_1},
	{"font_feature_stylistic_set_2", DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_2},
	{"font_feature_stylistic_set_3", DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_3},
	{"font_feature_stylistic_set_4", DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_4},
	{"font_feature_stylistic_set_5", DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_5},
	{"font_feature_stylistic_set_6", DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_6},
	{"font_feature_stylistic_set_7", DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_7},
	{"font_feature_stylistic_set_8", DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_8},
	{"font_feature_stylistic_set_9", DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_9},
	{"font_feature_stylistic_set_10", DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_10},
	{"font_feature_stylistic_set_11", DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_11},
	{"font_feature_stylistic_set_12", DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_12},
	{"font_feature_stylistic_set_13", DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_13},
	{"font_feature_stylistic_set_14", DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_14},
	{"font_feature_stylistic_set_15", DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_15},
	{"font_feature_stylistic_set_16", DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_16},
	{"font_feature_stylistic_set_17", DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_17},
	{"font_feature_stylistic_set_18", DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_18},
	{"font_feature_stylistic_set_19", DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_19},
	{"font_feature_stylistic_set_20", DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_20},
	{"font_feature_subscript", DWRITE_FONT_FEATURE_TAG_SUBSCRIPT},
	{"font_feature_superscript", DWRITE_FONT_FEATURE_TAG_SUPERSCRIPT},
	{"font_feature_swash", DWRITE_FONT_FEATURE_TAG_SWASH},
	{"font_feature_titling", DWRITE_FONT_FEATURE_TAG_TITLING},
	{"font_feature_traditional_name_forms", DWRITE_FONT_FEATURE_TAG_TRADITIONAL_NAME_FORMS},
	{"font_feature_traditional_forms", DWRITE_FONT_FEATURE_TAG_TRADITIONAL_FORMS},
	{"font_feature_third_widths", DWRITE_FONT_FEATURE_TAG_THIRD_WIDTHS},
	{"font_feature_unicase", DWRITE_FONT_FEATURE_TAG_UNICASE},
	{"font_feature_vertical_writing", DWRITE_FONT_FEATURE_TAG_VERTICAL_WRITING},
	{"font_feature_vertical_alternates_and_rotation", DWRITE_FONT_FEATURE_TAG_VERTICAL_ALTERNATES_AND_ROTATION},
	{"font_feature_slashed_zero", DWRITE_FONT_FEATURE_TAG_SLASHED_ZERO},

	// special name because im stupid Sorry
	{"tabular_figures", DWRITE_FONT_FEATURE_TAG_TABULAR_FIGURES},
};

bool operator<(const font_feature_tag& a, const font_feature_tag& b)
{
	return a.second < b.second;
}

bool operator<(const font_feature_tag& a, const DWRITE_FONT_FEATURE_TAG& b)
{
	return a.second < b;
}

bool operator<(const DWRITE_FONT_FEATURE_TAG& a, const font_feature_tag& b)
{
	return a < b.second;
}

static void upgrade_properties(void* data, obs_data_t* settings);

/* ------------------------------------------------------------------------- */

static inline DWORD get_alpha_val(uint32_t opacity)
{
	return ((opacity * 255 / 100) & 0xFF) << 24;
}

static inline DWORD calc_color(uint32_t color, uint32_t opacity)
{
	return color & 0xFFFFFF | get_alpha_val(opacity);
}

static inline std::wstring to_wide(const char* utf8)
{
	std::wstring text;

	size_t len = os_utf8_to_wcs(utf8, 0, nullptr, 0);
	text.resize(len);
	if (len)
		os_utf8_to_wcs(utf8, 0, &text[0], len + 1);

	return text;
}

static inline uint32_t rgb_to_bgr(uint32_t rgb)
{
	return ((rgb & 0xFF) << 16) | (rgb & 0xFF00) | ((rgb & 0xFF0000) >> 16);
}

static time_t get_modified_timestamp(const char* filename)
{
	struct stat stats;
	if (os_stat(filename, &stats) != 0)
		return -1;
	return stats.st_mtime;
}

void obs_dwrite_text_source::init_dwrite()
{
	obs_graphics_t gs;

	if (gs_get_device_type() != GS_DEVICE_DIRECT3D_11) {
		error("Unsupported render backend! Are you using OpenGL?");
		throw winrt::hresult_error(CO_E_NOT_SUPPORTED);
	}

	winrt::check_hresult(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, pD2DFactory.put()));

	// manually AddRef, OBS does not do this for us :)
	auto d3dDevice = reinterpret_cast<ID3D11Device*>(gs_get_device_obj());
	d3dDevice->AddRef();

	winrt::com_ptr<ID3D11Device> device(d3dDevice, winrt::take_ownership_from_abi);

	if (device == nullptr)
		throw winrt::hresult_error(E_UNEXPECTED);

	winrt::com_ptr<IDXGIDevice> dxgi = device.as<IDXGIDevice>();
	winrt::com_ptr<ID2D1Device> d2dDevice;
	winrt::check_hresult(pD2DFactory->CreateDevice(dxgi.get(), d2dDevice.put()));

	pD2DDevice = d2dDevice.as<ID2D1Device1>();
	winrt::check_hresult(
		pD2DDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, pD2DContext.put()));

	winrt::check_hresult(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory2),
		reinterpret_cast<IUnknown**>(pDWriteFactory.put())));

	auto pFactory6 = pDWriteFactory.try_as<IDWriteFactory6>();
	if (pFactory6) {
		winrt::com_ptr<IDWriteFontCollection2> pCollection2;
		pFactory6->GetSystemFontCollection(FALSE, DWRITE_FONT_FAMILY_MODEL_TYPOGRAPHIC, pCollection2.put());

		this->pSystemFontCollection = pCollection2.as<IDWriteFontCollection>();
	}
	else {
		pDWriteFactory->GetSystemFontCollection(pSystemFontCollection.put());
	}
}

gradient_axis_t obs_dwrite_text_source::calculate_gradient_axis(float width, float height) const
{
	if (width <= 0.0f || height <= 0.0f)
		return { 0.0f, 0.0f, 0.0f, 0.0f };

	float angle = (float)(atan(height / width) * 180.0f / M_PI);
	if (gradient_angle <= angle || gradient_angle > 360.0 - angle) {
		float y = (float)(width / 2 * tan(gradient_angle * M_PI / 180.0));
		return { width, height / 2.0f - y, 0, height / 2.0f + y };
	}
	else if (gradient_angle <= 180.0 - angle && gradient_angle > angle) {
		float x = (float)(height / 2 * tan((90.0 - gradient_angle) * M_PI / 180.0));
		return { width / 2.0f + x, 0, width / 2.0f - x, height };
	}
	else if (gradient_angle <= 180.0 + angle && gradient_angle > 180.0 - angle) {
		float y = (float)(width / 2 * tan(gradient_angle * M_PI / 180.0));
		return { 0, height / 2.0f + y, width, height / 2.0f - y };
	}
	else {
		float x = (float)(height / 2 * tan((270.0 - gradient_angle) * M_PI / 180.0));
		return { width / 2.0f - x, height, width / 2.0f + x, 0.0f };
	}
}

bool obs_dwrite_text_source::create_render_target_d3d11(usize_t& size, thickness_t& padding)
{
	obs_graphics_t gs;

	pD2DContext->SetTarget(nullptr);
	pTarget = nullptr;
	pTextTarget = nullptr;

	if (targetTexture) {
		gs_texture_destroy(targetTexture);
		targetTexture = nullptr;
	}
	if (textTexture) {
		gs_texture_destroy(textTexture);
		textTexture = nullptr;
	}

	auto width = size.width;
	auto height = size.height;
	auto pad_width = (uint32_t)std::ceilf(std::clamp(size.width + (padding.left + padding.right),
		(float)MIN_SIZE_CX, (float)MAX_SIZE_CX));
	auto pad_height = (uint32_t)std::ceilf(std::clamp(size.height + (padding.top + padding.bottom),
		(float)MIN_SIZE_CY, (float)MAX_SIZE_CY));

	if (!use_extents) {
		textTexture = gs_texture_create(width, height, GS_BGRA_UNORM, 1, nullptr, GS_RENDER_TARGET);
	}
	else {
		textTexture =
			gs_texture_create(pad_width, pad_height, GS_BGRA_UNORM, 1, nullptr, GS_RENDER_TARGET);
	}

	targetTexture = gs_texture_create(pad_width, pad_height, GS_BGRA_UNORM, 1, nullptr, GS_RENDER_TARGET);

	if (!targetTexture || !textTexture)
		return false;

	D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED), 96, 96);

	winrt::com_ptr<IDXGISurface> native_surface;
	reinterpret_cast<ID3D11Texture2D*>(gs_texture_get_obj(targetTexture))
		->QueryInterface(native_surface.put());

	HRESULT hr = pD2DContext->CreateBitmapFromDxgiSurface(native_surface.get(), &bitmapProperties,
		pTarget.put());

	if (FAILED(hr) || pTarget == nullptr)
		return false;

	reinterpret_cast<ID3D11Texture2D*>(gs_texture_get_obj(textTexture))
		->QueryInterface(native_surface.put());

	hr = pD2DContext->CreateBitmapFromDxgiSurface(native_surface.get(), &bitmapProperties,
		pTextTarget.put());

	if (FAILED(hr) || pTextTarget == nullptr)
		return false;

	return true;
}

void obs_dwrite_text_source::update_brush(float width, float height)
{
	obs_graphics_t gr;

	HRESULT hr;

	pFillBrush = nullptr;
	pOutlineBrush = nullptr;

	if (gradient_count > 1) {
		auto axis = calculate_gradient_axis(width, height);

		winrt::com_ptr<ID2D1GradientStopCollection> pGradientStops;

		float level = 1.0f / (gradient_count - 1);

		D2D1_GRADIENT_STOP* gradientStops = new D2D1_GRADIENT_STOP[gradient_count]{};
		for (size_t i = 0; i < gradient_count; i++) {
			gradientStops[i].color = D2D1::ColorF(gradient_stops[i].color, opacity / 100.0f);
			gradientStops[i].position = level * i;
		}

		hr = pD2DContext->CreateGradientStopCollection(gradientStops, (uint32_t)gradient_count,
			D2D1_GAMMA_2_2, D2D1_EXTEND_MODE_MIRROR,
			pGradientStops.put());

		hr = pD2DContext->CreateLinearGradientBrush(
			D2D1::LinearGradientBrushProperties(D2D1::Point2F(axis.x1, axis.y1),
				D2D1::Point2F(axis.x2, axis.y2)),
			pGradientStops.get(), (ID2D1LinearGradientBrush**)pFillBrush.put());

	}
	else {
		hr = pD2DContext->CreateSolidColorBrush(D2D1::ColorF(gradient_stops[0].color, opacity / 100.0f),
			(ID2D1SolidColorBrush**)pFillBrush.put());
	}

	if (use_outline) {
		hr = pD2DContext->CreateSolidColorBrush(D2D1::ColorF(outline_color, outline_opacity / 100.0f),
			(ID2D1SolidColorBrush**)pOutlineBrush.put());
	}
}

void obs_dwrite_text_source::update_effects()
{
	obs_graphics_t gr;

	pEffect = nullptr;

	if (use_shadow) {
		winrt::com_ptr<ID2D1Effect> shadowEffect;
		pD2DContext->CreateEffect(CLSID_D2D1Shadow, shadowEffect.put());

		shadowEffect->SetInput(0, pTextTarget.get());
		shadowEffect->SetValue(D2D1_SHADOW_PROP_BLUR_STANDARD_DEVIATION, shadow_radius / 3.0f);
		shadowEffect->SetValue(D2D1_SHADOW_PROP_COLOR,
			D2D1::ColorF(shadow_color, (shadow_opacity / 100.0f)));

		winrt::com_ptr<ID2D1Effect> affineTransformEffect;
		pD2DContext->CreateEffect(CLSID_D2D12DAffineTransform, affineTransformEffect.put());

		affineTransformEffect->SetInputEffect(0, shadowEffect.get());

		D2D1_MATRIX_3X2_F matrix = D2D1::Matrix3x2F::Translation(shadow_offset_x, shadow_offset_y);
		affineTransformEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, matrix);

		pD2DContext->CreateEffect(CLSID_D2D1Composite, pEffect.put());
		pEffect->SetInputEffect(0, affineTransformEffect.get());
		pEffect->SetInput(1, pTextTarget.get());
	}
}

bool obs_dwrite_text_source::create_text_layout(
	std::wstring& font_face,
	float weight,
	float stretch,
	DWRITE_FONT_STYLE style,
	UINT32 text_length,
	float layout_cx,
	float layout_cy)
{
	obs_graphics_t gr;

	std::wstring obs_locale = to_wide(obs_get_locale());

	try {

		winrt::com_ptr<IDWriteFactory6> pFactory6
			= this->pDWriteFactory.as<IDWriteFactory6>();

		winrt::com_ptr<IDWriteFontCollection> systemFontCollection;
		pFactory6->GetSystemFontCollection(systemFontCollection.put());

		winrt::com_ptr<IDWriteFontCollection2> fontCollection2
			= systemFontCollection.as<IDWriteFontCollection2>();

		std::vector<DWRITE_FONT_AXIS_VALUE> axisValues{ variables };

		winrt::com_ptr<IDWriteFontSet1> fontSet1;
		winrt::check_hresult(pFactory6->GetSystemFontSet(false, fontSet1.put()));

		winrt::com_ptr<IDWriteFontSet4> fontSet = fontSet1.try_as<IDWriteFontSet4>();
		if (fontSet) {
			size_t const inputAxisCount = axisValues.size();
			if (inputAxisCount > UINT32_MAX - DWRITE_STANDARD_FONT_AXIS_COUNT)
				winrt::check_hresult(E_INVALIDARG);

			axisValues.resize(inputAxisCount + DWRITE_STANDARD_FONT_AXIS_COUNT);

			UINT32 derivedAxisCount = fontSet->ConvertWeightStretchStyleToFontAxisValues(
				axisValues.data(),
				static_cast<UINT32>(inputAxisCount),
				(DWRITE_FONT_WEIGHT)weight,
				(DWRITE_FONT_STRETCH)stretch,
				style,
				(float)face_size / 96.0f * 72.0f,
				axisValues.data() + inputAxisCount
			);

			axisValues.resize(inputAxisCount + derivedAxisCount);
		}
		else {
			// TODO: shim for ConvertWeightStretchStyleToFontAxisValues
		}

		float weight_value = weight;
		float stretch_value = has_variables ? stretch : stretchToWidth[(int)stretch];
		float slant_value = style == DWRITE_FONT_STYLE_NORMAL ? 0 :
			style == DWRITE_FONT_STYLE_ITALIC ? -5 :
			-10;

		bool has_weight = false, has_stretch = false, has_slant = false, has_italic = false;
		for (auto& value : axisValues) {
			if (value.axisTag == DWRITE_FONT_AXIS_TAG_WEIGHT) {
				value.value = weight_value;
				has_weight = true;
			}

			if (value.axisTag == DWRITE_FONT_AXIS_TAG_WIDTH) {
				value.value = stretch_value;
				has_stretch = true;
			}

			if (value.axisTag == DWRITE_FONT_AXIS_TAG_SLANT) {
				has_slant = true;
			}

			if (value.axisTag == DWRITE_FONT_AXIS_TAG_ITALIC) {
				has_italic = true;
			}
		}

		if (!has_weight) {
			axisValues.push_back({
				DWRITE_FONT_AXIS_TAG_WEIGHT,
				weight_value
				});
		}

		if (!has_stretch) {
			axisValues.push_back({
				DWRITE_FONT_AXIS_TAG_WIDTH,
				stretch_value
				});
		}

		if (style != DWRITE_FONT_STYLE_NORMAL) {
			if (!has_slant && !has_italic) {
				if (style == DWRITE_FONT_STYLE_ITALIC) {
					axisValues.push_back({
						DWRITE_FONT_AXIS_TAG_ITALIC,
						1.0f
						});
				}

				axisValues.push_back({
					DWRITE_FONT_AXIS_TAG_SLANT,
					slant_value
					});
			}
		}

		if (!has_variables) {
			winrt::com_ptr<IDWriteFontList2> matchingFonts;
			winrt::check_hresult(fontCollection2->GetMatchingFonts(
				font_face.c_str(),
				axisValues.data(),
				static_cast<UINT32>(axisValues.size()),
				matchingFonts.put()
			));

			winrt::com_ptr<IDWriteFontFaceReference> faceRef;
			winrt::check_hresult(matchingFonts->GetFontFaceReference(0, faceRef.put()));

			winrt::com_ptr<IDWriteFontFace3> fontFace;
			winrt::check_hresult(faceRef->CreateFontFace(fontFace.put()));

			winrt::com_ptr<IDWriteLocalizedStrings> pFamilyNames;
			winrt::check_hresult(fontFace->GetFamilyNames(pFamilyNames.put()));

			std::wstring name{};

			UINT32 index = 0;
			UINT32 length = 0;
			HRESULT hr = S_OK;
			BOOL exists = false;
			if (SUCCEEDED(hr = pFamilyNames->FindLocaleName(L"en-us", &index, &exists)) && exists) {
				pFamilyNames->GetStringLength(index, &length);
				name.resize(length);
				pFamilyNames->GetString(index, name.data(), length + 1);
			}

			font_face = name;
		}

		info("Creating text layout (modern) with font: %s, size %.2f", winrt::to_string(font_face).c_str(), (float)face_size / 96.0f * 72.0f);

		winrt::com_ptr<IDWriteTextFormat3> pTextFormat3;
		winrt::check_hresult(pFactory6->CreateTextFormat(
			font_face.c_str(), NULL, axisValues.data(), axisValues.size(), (float)face_size / 96.0f * 72.0f,
			obs_locale.c_str(), pTextFormat3.put()));

		winrt::check_hresult(pFactory6->CreateTextLayout(text.c_str(), text_length, pTextFormat3.get(), layout_cx, layout_cy, pTextLayout.put()));

		pTextLayout->SetTextAlignment(align);
		pTextLayout->SetParagraphAlignment(valign);
		pTextLayout->SetWordWrapping(wrap);

		if (this->line_spacing != S_LINE_SPACING_DEFAULT) {
			pTextLayout->SetLineSpacing(line_spacing, line_spacing_ratio, line_spacing_ratio * 0.8f);
		}
	}
	catch (winrt::hresult_error& e) {
		error("Modern text layout failed with code %#08x (%s)", e.code().value,
			to_string(e.message()).c_str());

		if (FAILED(pDWriteFactory->CreateTextFormat(
			font_face.c_str(),
			NULL, (DWRITE_FONT_WEIGHT)weight, style, (DWRITE_FONT_STRETCH)stretch, (float)face_size / 96.0f * 72.0f,
			obs_locale.c_str(), pTextFormat.put())))
			return false;

		pTextFormat->SetTextAlignment(align);
		pTextFormat->SetParagraphAlignment(valign);
		pTextFormat->SetWordWrapping(wrap);

		if (this->line_spacing != S_LINE_SPACING_DEFAULT) {
			pTextFormat->SetLineSpacing(line_spacing, line_spacing_ratio, line_spacing_ratio * 0.8f);
		}

		if (FAILED(pDWriteFactory->CreateTextLayout(text.c_str(), text_length, pTextFormat.get(), layout_cx, layout_cy, pTextLayout.put())))
			return false;
	}

	return true;
}

void obs_dwrite_text_source::draw_text()
{
	obs_graphics_t gr;

	pTextRenderer = nullptr;
	pTextFormat = nullptr;
	pTextLayout = nullptr;

	auto weight = this->weight;
	auto stretch = this->stretch;
	auto style = this->style;

	UINT32 text_length = (UINT32)text.size();
	HRESULT hr = S_OK;

	usize_t calculated_size = { 0, 0 };

	// calculate padding
	float top = 0, bottom = 0, left = 0, right = 0;
	if (use_shadow) {
		top = shadow_radius / 2.0f;
		bottom = shadow_radius / 2.0f;
		left = shadow_radius / 2.0f;
		right = shadow_radius / 2.0f;

		top -= shadow_offset_y;
		bottom += shadow_offset_y;
		left -= shadow_offset_x;
		right += shadow_offset_x;
	}

	thickness_t calculated_padding = { max(0, top), max(0, left), max(0, bottom), max(0, right) };

	if (use_outline) {
		calculated_padding.top += outline_size;
		calculated_padding.left += outline_size;
		calculated_padding.bottom += outline_size;
		calculated_padding.right += outline_size;
	}

	float layout_cx = (use_extents && extents_cx != -1)
		? (float)extents_cx - (calculated_padding.left + calculated_padding.right)
		: INFINITY;
	float layout_cy = (use_extents && extents_cy != -1)
		? (float)extents_cy - (calculated_padding.top + calculated_padding.bottom)
		: INFINITY;

	float text_cx = 0.0f;
	float text_cy = 0.0f;
	UINT32 lines = 1;

	if (weight == S_FONT_WEIGHT_AUTO) {
		weight = S_FONT_WEIGHT_400;
	}

	if (stretch == S_FONT_STRETCH_AUTO) {
		stretch = S_FONT_STRETCH_NORMAL;
	}

	std::wstring font_face = this->face;
	if (!create_text_layout(font_face, weight, stretch, style, text_length, layout_cx, layout_cy))
		return;

	DWRITE_TEXT_RANGE text_range = { 0, text_length };
	pTextLayout->SetUnderline(underline, text_range);
	pTextLayout->SetStrikethrough(strikeout, text_range);
	pTextLayout->SetWordWrapping(wrap);

	if (!this->font_features.empty()) {
		winrt::com_ptr<IDWriteTypography> pTypography;
		if (SUCCEEDED(pDWriteFactory->CreateTypography(pTypography.put()))) {
			for (auto [featureTag, value] : this->font_features) {
				pTypography->AddFontFeature(DWRITE_FONT_FEATURE{ featureTag.second, value });
			}
			pTextLayout->SetTypography(pTypography.get(), text_range);
		}
	}

	for (auto&& run : runs) {
		DWRITE_TEXT_RANGE run_range = { run.start, run.length };

		if (run.format == format_flags::bold) {
			pTextLayout->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD, run_range);
		}

		if (run.format == format_flags::italic) {
			pTextLayout->SetFontStyle(DWRITE_FONT_STYLE_ITALIC, run_range);
		}

		if (run.format == format_flags::underline) {
			pTextLayout->SetUnderline(true, run_range);
		}

		if (run.format == format_flags::strikethrough) {
			pTextLayout->SetStrikethrough(true, run_range);
		}

		if (run.size.has_value()) {
			pTextLayout->SetFontSize(run.size.value(), run_range);
		}
	}

	if (text_trimming != S_TRIMMING_NONE) {
		winrt::com_ptr<IDWriteInlineObject> inlineObject;
		if (SUCCEEDED(pDWriteFactory->CreateEllipsisTrimmingSign(pTextFormat.get(), inlineObject.put()))) {
			DWRITE_TRIMMING trimming = { (text_trimming == S_TRIMMING_CHARACTER_ELLIPSIS
								 ? DWRITE_TRIMMING_GRANULARITY_CHARACTER
								 : DWRITE_TRIMMING_GRANULARITY_WORD),
							0, 0 };

			pTextLayout->SetTrimming(&trimming, inlineObject.get());
		}
	}
	else {
		DWRITE_TRIMMING trimming = { DWRITE_TRIMMING_GRANULARITY_NONE, 0, 0 };
		pTextLayout->SetTrimming(&trimming, nullptr);
	}

	pTextLayout->SetMaxWidth(layout_cx);
	pTextLayout->SetMaxHeight(layout_cy);

	DWRITE_TEXT_METRICS textMetrics;
	if (FAILED(hr = pTextLayout->GetMetrics(&textMetrics))) {
		error("pTextLayout->GetMetrics(&textMetrics) failed with code %#08x", hr);
		return;
	}

	text_cx = ceil(textMetrics.widthIncludingTrailingWhitespace);
	text_cy = ceil(textMetrics.height);
	lines = textMetrics.lineCount;

	if (isnan(text_cx))
		text_cx = MAX_SIZE_CX;
	if (isnan(text_cy))
		text_cy = MAX_SIZE_CY;

	if (!use_extents || extents_cx == -1) {
		layout_cx = text_cx;
	}

	if (!use_extents || extents_cy == -1) {
		layout_cy = text_cy;
	}

	clamp_assign(layout_cx, MIN_SIZE_CX, MAX_SIZE_CX);
	clamp_assign(layout_cy, MIN_SIZE_CY, MAX_SIZE_CY);

	calculated_size = { (uint32_t)layout_cx, (uint32_t)layout_cy };

	if (!pTarget || !targetTexture || size != calculated_size || calculated_padding != padding) {
		if (!create_render_target_d3d11(calculated_size, calculated_padding)) {
			return;
		}

		size = calculated_size;
		padding = calculated_padding;
	}

	update_brush(text_cx, text_cy / lines);

	pTextRenderer.attach(new OBSTextRenderer(pDWriteFactory.get(), pD2DFactory.get(),
		pD2DContext.get(), pOutlineBrush.get(),
		pFillBrush.get(), outline_size, color_fonts));

	pD2DContext->BeginDraw();
	pD2DContext->SetTarget(pTextTarget.get());
	pD2DContext->SetTransform(D2D1::IdentityMatrix());
	pD2DContext->Clear(D2D1::ColorF(0, 0, 0, 0));

	pD2DContext->SetTextAntialiasMode(antialias ? D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE
		: D2D1_TEXT_ANTIALIAS_MODE_ALIASED);

	pTextLayout->Draw(NULL, pTextRenderer.get(), 0, 0);

	hr = pD2DContext->EndDraw();
	if (FAILED(hr)) {
		error("pD2DContext->EndDraw() failed with code %#08x", hr);
		return;
	}

	update_effects();

	pD2DContext->BeginDraw();
	pD2DContext->SetTarget(pTarget.get());
	pD2DContext->SetTransform(D2D1::IdentityMatrix());
	pD2DContext->Clear(D2D1::ColorF(bk_color, bk_opacity / 100.0f));

	if (pEffect) {
		pD2DContext->DrawImage(pEffect.get(), D2D1::Point2F(padding.left, padding.top));
	}
	else {
		pD2DContext->DrawImage(pTextTarget.get(), D2D1::Point2F(padding.left, padding.top));
	}

	hr = pD2DContext->EndDraw();
	if (FAILED(hr)) {
		error("pD2DContext->EndDraw() failed with code %#08x", hr);
		return;
	}
}

void obs_dwrite_text_source::transform_text()
{
	const std::locale loc = std::locale(obs_get_locale());
	const std::ctype<wchar_t>& f = std::use_facet<std::ctype<wchar_t>>(loc);

	if (text_transform == S_TRANSFORM_UPPERCASE)
		f.toupper(&text[0], &text[0] + text.size());
	else if (text_transform == S_TRANSFORM_LOWERCASE)
		f.tolower(&text[0], &text[0] + text.size());
	else if (text_transform == S_TRANSFORM_STARTCASE) {
		bool upper = true;
		for (auto it = text.begin(); it != text.end(); ++it) {
			const wchar_t upper_char = f.toupper(*it);
			const wchar_t lower_char = f.tolower(*it);
			if (upper && lower_char != upper_char) {
				upper = false;
				*it = upper_char;
			}
			else if (lower_char != upper_char) {
				*it = lower_char;
			}
			else {
				upper = iswspace(*it);
			}
		}
	}

	runs.clear();
}

const char* obs_dwrite_text_source::get_string(const char* str)
{
	if (!str)
		return "";
	if (!chatlog_mode || !chatlog_lines)
		return str;

	int lines = chatlog_lines;
	size_t len = strlen(str);
	if (!len)
		return str;

	const char* temp = str + len;

	while (temp != str) {
		temp--;

		if (temp[0] == '\n' && temp[1] != 0) {
			if (!--lines)
				break;
		}
	}

	return *temp == '\n' ? temp + 1 : temp;
}

void obs_dwrite_text_source::load_file()
{
	BPtr<char> file_text = os_quick_read_utf8_file(file.c_str());
	text = to_wide(get_string(file_text));
}

#define obs_data_get_uint32 (uint32_t) obs_data_get_int
#define obs_data_get_int32 (int32_t) obs_data_get_int

inline void obs_dwrite_text_source::Update(obs_data_t* s)
{
	if (!pD2DContext)
		return;

	upgrade_properties(this, s);

	has_variables = obs_data_get_bool(s, S_HAS_VARIABLES);

	const char* new_text = obs_data_get_string(s, S_TEXT);
	const char* align_str = obs_data_get_string(s, S_ALIGN);
	const char* valign_str = obs_data_get_string(s, S_VALIGN);
	const char* wrap_str = obs_data_get_string(s, S_WRAP_MODE);
	uint32_t new_opacity = obs_data_get_uint32(s, S_OPACITY);

	bool new_outline = obs_data_get_bool(s, S_OUTLINE);
	uint32_t new_o_color = obs_data_get_uint32(s, S_OUTLINE_COLOR);
	uint32_t new_o_opacity = obs_data_get_uint32(s, S_OUTLINE_OPACITY);
	uint32_t new_o_size = obs_data_get_uint32(s, S_OUTLINE_SIZE);

	bool new_use_file = obs_data_get_bool(s, S_USE_FILE);
	const char* new_file = obs_data_get_string(s, S_FILE);

	bool new_chat_mode = obs_data_get_bool(s, S_CHATLOG_MODE);
	int32_t new_chat_lines = obs_data_get_int32(s, S_CHATLOG_LINES);

	bool new_extents = obs_data_get_bool(s, S_EXTENTS);
	int32_t n_extents_cx = (int32_t)obs_data_get_int(s, S_EXTENTS_CX);
	int32_t n_extents_cy = (int32_t)obs_data_get_int(s, S_EXTENTS_CY);

	const char* new_font_face = obs_data_get_string(s, S_FONT_FACE);
	const char* new_font_style = obs_data_get_string(s, S_FONT_STYLE);
	int32_t new_font_size = obs_data_get_int32(s, S_FONT_SIZE);

	float new_font_weight = has_variables ? obs_data_get_double(s, S_FONT_WEIGHT "_variable") : obs_data_get_int32(s, S_FONT_WEIGHT);
	float new_font_stretch = has_variables ? obs_data_get_double(s, S_FONT_STRETCH "_variable") : obs_data_get_int32(s, S_FONT_STRETCH);

	bool new_underline = obs_data_get_bool(s, S_FONT_UNDERLINE);
	bool new_strikeout = obs_data_get_bool(s, S_FONT_STRIKETHROUGH);

	bool new_color_fonts = obs_data_get_bool(s, S_COLOR_FONTS);
	bool new_antialias = obs_data_get_bool(s, S_ANTIALIASING);

	int32_t new_text_transform = obs_data_get_int32(s, S_TRANSFORM);
	int32_t new_text_trimming = obs_data_get_int32(s, S_TRIMMING);

	uint32_t new_bk_color = obs_data_get_uint32(s, S_BKCOLOR);
	uint32_t new_bk_opacity = obs_data_get_uint32(s, S_BKOPACITY);

	int32_t new_gradient_count = obs_data_get_int32(s, S_GRADIENT_EX_COUNT);
	float new_grad_dir = (float)obs_data_get_double(s, S_GRADIENT_DIR);

	bool new_shadow = obs_data_get_bool(s, S_SHADOW);
	float new_shadow_offset_x = (float)obs_data_get_double(s, S_SHADOW_OFFSET_X);
	float new_shadow_offset_y = (float)obs_data_get_double(s, S_SHADOW_OFFSET_Y);
	float new_shadow_radius = (float)obs_data_get_double(s, S_SHADOW_RADIUS);
	uint32_t new_shadow_color = obs_data_get_uint32(s, S_SHADOW_COLOR);
	uint32_t new_shadow_opacity = obs_data_get_uint32(s, S_SHADOW_OPACITY);

	int32_t new_line_spacing = obs_data_get_int32(s, S_LINE_SPACING);
	float new_line_spacing_ratio = (float)obs_data_get_double(s, S_LINE_SPACING_RATIO);

	auto new_auto_optical_size = obs_data_get_bool(s, S_AUTO_OPTICAL_SIZE);

	std::vector<DWRITE_FONT_AXIS_VALUE> axis{};

	obs_data_item_t* item = obs_data_first(s);
	do {
		std::string name = obs_data_item_get_name(item);
		if (name.rfind("variable_prop::", 0) != 0 || name == S_AUTO_OPTICAL_SIZE) {
			continue;
		}

		name = name.substr(15);

		DWRITE_FONT_AXIS_VALUE value{};
		value.axisTag = DWRITE_MAKE_FONT_AXIS_TAG(name[0], name[1], name[2], name[3]);
		value.value = obs_data_item_get_double(item);

		if (value.axisTag != DWRITE_FONT_AXIS_TAG_OPTICAL_SIZE || !new_auto_optical_size)
			axis.push_back(value);

		info("property name: %s value: %.1f", name.c_str(), value.value);
	} while (obs_data_item_next(&item));

	variables = { axis };

	std::wstring new_face = to_wide(new_font_face);

	face = new_face;
	face_size = new_font_size;
	weight = new_font_weight;
	font_style = new_font_style;
	underline = new_underline;
	strikeout = new_strikeout;
	stretch = new_font_stretch;

	if (font_style == S_FONT_STYLE_ITALIC)
		style = DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_ITALIC;
	else if (font_style == S_FONT_STYLE_OBLIQUE)
		style = DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_OBLIQUE;
	else
		style = DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL;

	line_spacing = (DWRITE_LINE_SPACING_METHOD)new_line_spacing;
	line_spacing_ratio = new_line_spacing_ratio;

	new_o_color = rgb_to_bgr(new_o_color);
	new_bk_color = rgb_to_bgr(new_bk_color);
	new_shadow_color = rgb_to_bgr(new_shadow_color);

	opacity = new_opacity;
	gradient_angle = new_grad_dir;
	color_fonts = new_color_fonts;
	antialias = new_antialias;
	text_transform = new_text_transform;
	text_trimming = new_text_trimming;

	gradient_count = new_gradient_count;
	for (size_t i = 0; i < new_gradient_count; i++) {
		gradient_stops[i].color = rgb_to_bgr(obs_data_get_uint32(s, gradient_stop_color_names[i]));
	}

	bk_color = new_bk_color;
	bk_opacity = new_bk_opacity;
	use_extents = new_extents;
	extents_cx = n_extents_cx;
	extents_cy = n_extents_cy;

	read_from_file = new_use_file;

	chatlog_mode = new_chat_mode;
	chatlog_lines = new_chat_lines;

	if (read_from_file) {
		file = new_file;
		file_timestamp = get_modified_timestamp(new_file);
		load_file();
	}
	else {
		text = to_wide(get_string(new_text));
	}

	transform_text();

	use_outline = new_outline;
	outline_color = new_o_color;
	outline_opacity = new_o_opacity;
	outline_size = roundf(float(new_o_size));

	use_shadow = new_shadow;
	shadow_color = new_shadow_color;
	shadow_offset_x = new_shadow_offset_x;
	shadow_offset_y = new_shadow_offset_y;
	shadow_radius = new_shadow_radius;
	shadow_opacity = new_shadow_opacity;

	if (strcmp(align_str, S_ALIGN_CENTER) == 0)
		align = DWRITE_TEXT_ALIGNMENT_CENTER;
	else if (strcmp(align_str, S_ALIGN_RIGHT) == 0)
		align = DWRITE_TEXT_ALIGNMENT_TRAILING;
	else if (strcmp(align_str, S_ALIGN_JUSTIFIED) == 0)
		align = DWRITE_TEXT_ALIGNMENT_JUSTIFIED;
	else
		align = DWRITE_TEXT_ALIGNMENT_LEADING;

	if (strcmp(valign_str, S_VALIGN_CENTER) == 0)
		valign = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
	else if (strcmp(valign_str, S_VALIGN_BOTTOM) == 0)
		valign = DWRITE_PARAGRAPH_ALIGNMENT_FAR;
	else
		valign = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;

	if (strcmp(wrap_str, S_WRAP_MODE_WRAP_WHOLE_WORDS) == 0)
		wrap = DWRITE_WORD_WRAPPING_WHOLE_WORD;
	else if (strcmp(wrap_str, S_WRAP_MODE_WRAP_CHARACTER) == 0)
		wrap = DWRITE_WORD_WRAPPING_CHARACTER;
	else if (strcmp(wrap_str, S_WRAP_MODE_WRAP) == 0)
		wrap = DWRITE_WORD_WRAPPING_WRAP;
	else if (strcmp(wrap_str, S_WRAP_MODE_NONE) == 0)
		wrap = DWRITE_WORD_WRAPPING_NO_WRAP;

	// Build font_features set from settings
	this->font_features.clear();
	for (const auto& entry : font_feature_set) {
		auto value = obs_data_get_bool(s, entry.first);
		if (value > 0) {
			this->font_features.insert_or_assign(entry, (UINT32)value);
		}
	}

	draw_text();
	update_time_elapsed = 0.0f;
}

inline void obs_dwrite_text_source::Tick(float seconds)
{
	if (!pD2DContext)
		return;

	if (!read_from_file)
		return;

	update_time_elapsed += seconds;

	if (update_time_elapsed >= 1.0f) {
		time_t t = get_modified_timestamp(file.c_str());
		update_time_elapsed = 0.0f;

		if (file_timestamp != t) {
			load_file();
			transform_text();
			draw_text();
			file_timestamp = t;
		}
	}
}

inline void obs_dwrite_text_source::Render(gs_effect_t*)
{
	if (!pD2DContext)
		return;

	if (!targetTexture)
		return;

	obs_graphics_t gs;

	gs_texture_t* texture = targetTexture;
	gs_effect_t* effect = obs_get_base_effect(OBS_EFFECT_DEFAULT);
	gs_technique_t* tech = gs_effect_get_technique(effect, "Draw");
	gs_eparam_t* image = gs_effect_get_param_by_name(effect, "image");

	gs_blend_state_push();
	gs_blend_function(GS_BLEND_ONE, GS_BLEND_INVSRCALPHA);

	const bool previous = gs_framebuffer_srgb_enabled();
	gs_enable_framebuffer_srgb(false);

	size_t passes = gs_technique_begin(tech);

	for (size_t i = 0; i < passes; i++) {
		if (gs_technique_begin_pass(tech, i)) {
			gs_effect_set_texture(image, texture);
			gs_draw_sprite(texture, 0, 0, 0);
			gs_technique_end_pass(tech);
		}
	}

	gs_technique_end(tech);

	gs_enable_framebuffer_srgb(previous);
	gs_blend_state_pop();
}

/* ------------------------------------------------------------------------- */

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("text-directwrite", "en-US")
MODULE_EXPORT const char* obs_module_description(void)
{
	return "Windows DirectWrite/Direct2D text source";
}

/* clang-format off */

#define set_vis(var, val, show)						\
	do {											\
		p = obs_properties_get(props, val);			\
		obs_property_set_visible(p, var == show);	\
	} while (false)

/* clang-format on */

static inline int width_to_stretch(float width)
{
	int best = 0;
	float bestDiff = fabsf(width - stretchToWidth[0]);

	for (int i = 1; i < 10; i++) {
		float diff = fabsf(width - stretchToWidth[i]);
		if (diff < bestDiff) {
			best = i;
			bestDiff = diff;
		}
	}

	return best;
}

static inline bool is_standard_axis(DWRITE_FONT_AXIS_TAG tag)
{
	// allow adjustment of optical size, we dont really know the 1:1 scale at which text will appear on screen
	return tag == DWRITE_FONT_AXIS_TAG_WEIGHT ||
		tag == DWRITE_FONT_AXIS_TAG_WIDTH ||
		tag == DWRITE_FONT_AXIS_TAG_ITALIC ||
		tag == DWRITE_FONT_AXIS_TAG_SLANT /*||
		tag == DWRITE_FONT_AXIS_TAG_OPTICAL_SIZE*/;
}

static bool use_file_changed(obs_properties_t* props, obs_property_t* p, obs_data_t* s)
{
	bool use_file = obs_data_get_bool(s, S_USE_FILE);

	set_vis(use_file, S_TEXT, false);
	set_vis(use_file, S_FILE, true);
	return true;
}

static bool auto_optical_size_changed(obs_properties_t* props, obs_property_t* p, obs_data_t* s)
{
	bool auto_size = obs_data_get_bool(s, S_AUTO_OPTICAL_SIZE);

	set_vis(auto_size, S_OPTICAL_SIZE, false);
	return true;
}


static bool font_changed(void* priv, obs_properties_t* props, obs_property_t* p, obs_data_t* s) {

	obs_dwrite_text_source* src = reinterpret_cast<obs_dwrite_text_source*>(priv);
#define this src

	std::wstring familyName = to_wide(obs_data_get_string(s, S_FONT_FACE));
	std::string font_style{ obs_data_get_string(s, S_FONT_STYLE) };

	DWRITE_FONT_WEIGHT weight{ (DWRITE_FONT_WEIGHT)obs_data_get_int32(s, S_FONT_WEIGHT) };
	DWRITE_FONT_STRETCH stretch{ (DWRITE_FONT_STRETCH)obs_data_get_int32(s, S_FONT_STRETCH) };
	DWRITE_FONT_STYLE style{};

	if (font_style == S_FONT_STYLE_ITALIC)
		style = DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_ITALIC;
	else if (font_style == S_FONT_STYLE_OBLIQUE)
		style = DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_OBLIQUE;
	else
		style = DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL;

	UINT32 familyIndex = 0;
	BOOL exists = false;
	if (FAILED(src->pSystemFontCollection->FindFamilyName(familyName.c_str(), &familyIndex, &exists)) || !exists)
		return false;

	winrt::com_ptr<IDWriteFontFamily> fontFamily;
	if (FAILED(src->pSystemFontCollection->GetFontFamily(familyIndex, fontFamily.put())))
		return false;

	winrt::com_ptr<IDWriteFont> font;
	if (FAILED(fontFamily->GetFirstMatchingFont(weight, stretch, style, font.put())))
		return false;

	winrt::com_ptr<IDWriteFontFace> fontFace;
	if (FAILED(font->CreateFontFace(fontFace.put())))
		return false;

	// Hide all feature properties first, and then update if supported, to avoid showing
	// unsupported features when switching between fonts with different feature support
	for (const auto& entry : font_feature_set) {
		obs_property_t* feat_prop = obs_properties_get(props, entry.first);
		obs_property_set_visible(feat_prop, false);
	}

	auto pFactory = src->pDWriteFactory.as<IDWriteFactory>();
	winrt::com_ptr<IDWriteTextAnalyzer> pTextAnalyzer;
	winrt::check_hresult(pFactory->CreateTextAnalyzer(pTextAnalyzer.put()));
	auto pTextAnalyzer2 = pTextAnalyzer.try_as<IDWriteTextAnalyzer2>();
	if (pTextAnalyzer2) {
		std::vector<DWRITE_FONT_FEATURE_TAG> featureTags = {};
		UINT32 featureCount = 0;
		HRESULT hr = pTextAnalyzer2->GetTypographicFeatures(
			fontFace.get(), DWRITE_SCRIPT_ANALYSIS{ 0, DWRITE_SCRIPT_SHAPES_DEFAULT },
			nullptr, 0, &featureCount, featureTags.data());

		if (featureCount > 0) {
			featureTags.resize(featureCount);
			hr = pTextAnalyzer2->GetTypographicFeatures(
				fontFace.get(), DWRITE_SCRIPT_ANALYSIS{ 0, DWRITE_SCRIPT_SHAPES_DEFAULT },
				nullptr, featureCount, &featureCount, featureTags.data());
			blog(LOG_INFO, "font_changed: Font supports %u typographic features", featureCount);

			// Toggle visibility of each known font-feature property based on support
			for (const auto featureTag : featureTags) {
				const auto& key = font_feature_set.find(featureTag);
				if (key != font_feature_set.end()) {
					obs_property_t* feat_prop = obs_properties_get(props, key->first);
					if (feat_prop != nullptr) {
						obs_property_set_visible(feat_prop, true);
					}
					else {
						blog(LOG_WARNING, "font_changed: Failed to find property for supported font feature with tag '%c%c%c%c'",
							(featureTag & 0xFF), ((featureTag >> 8) & 0xFF),
							((featureTag >> 16) & 0xFF), ((featureTag >> 24) & 0xFF));
					}
				}
				else {
					blog(LOG_WARNING, "font_changed: Font supports unknown typographic feature with tag '%c%c%c%c'",
						(featureTag & 0xFF), ((featureTag >> 8) & 0xFF),
						((featureTag >> 16) & 0xFF), ((featureTag >> 24) & 0xFF));
				}
			}
		}
		else {
			blog(LOG_INFO, "font_changed: Font does not support any typographic features? %u",
				featureCount);
		}
	}

	obs_property_t* variable_props = obs_properties_get(props, S_VARIABLE);
	obs_properties_t* variable_group = obs_property_group_content(variable_props);

	auto fontFace5 = fontFace.try_as<IDWriteFontFace5>();
	if (fontFace5 && fontFace5->HasVariations())
	{
		obs_property_set_visible(variable_props, true);

		obs_property_t* first;
		while ((first = obs_properties_first(variable_group))) {
			obs_properties_remove_by_name(variable_group, obs_property_name(first));
		}

		winrt::com_ptr<IDWriteFontResource> fontResource;
		fontFace5->GetFontResource(fontResource.put());

		UINT32 axisCount = fontFace5->GetFontAxisValueCount();
		std::vector<DWRITE_FONT_AXIS_VALUE> axisValues(axisCount);
		std::vector<DWRITE_FONT_AXIS_RANGE> axisRanges(axisCount);

		fontFace5->GetFontAxisValues(axisValues.data(), axisCount);
		fontResource->GetFontAxisRanges(axisRanges.data(), axisCount);

		for (UINT32 i = 0; i < axisCount; i++) {
			DWRITE_FONT_AXIS_TAG t = axisValues[i].axisTag;
			char tagStr[5] = {
				(char)(t & 0xFF),
				(char)((t >> 8) & 0xFF),
				(char)((t >> 16) & 0xFF),
				(char)((t >> 24) & 0xFF),
				'\0'
			};

			std::string tag{ tagStr };

			std::string translationName = "Axis." + tag;
			std::string displayName = T_(translationName.c_str());

			float current = axisValues[i].value;
			float minVal = axisRanges[i].minValue;
			float maxVal = axisRanges[i].maxValue;

			// e.g. "wght: 400.0 [100.0 - 900.0]"
			info("axis '%s': %.1f [%.1f - %.1f]", tagStr, current, minVal, maxVal);

			if (is_standard_axis(t)) {
				if (t == DWRITE_FONT_AXIS_TAG_WEIGHT) {
					obs_property_float_set_limits(obs_properties_get(props, S_FONT_WEIGHT "_variable"), minVal, maxVal, 0.1);
					obs_property_set_visible(obs_properties_get(props, S_FONT_WEIGHT "_variable"), minVal != maxVal);
				}

				if (t == DWRITE_FONT_AXIS_TAG_WIDTH) {
					obs_property_float_set_limits(obs_properties_get(props, S_FONT_STRETCH "_variable"), minVal, maxVal, 0.1);
					obs_property_set_visible(obs_properties_get(props, S_FONT_STRETCH "_variable"), minVal != maxVal);
				}

				continue;
			}

			if (minVal == maxVal)
				continue;

			auto name = ("variable_prop::" + tag);

			if (t == DWRITE_FONT_AXIS_TAG_OPTICAL_SIZE) {
				auto p = obs_properties_add_bool(variable_group, S_AUTO_OPTICAL_SIZE, T_AUTO_OPTICAL_SIZE);
				obs_property_set_modified_callback(p, auto_optical_size_changed);
			}

			obs_data_set_double(s, name.c_str(), current);
			auto slider = obs_properties_add_float_slider(variable_group, name.c_str(), displayName.c_str(), minVal, maxVal, 0.1);

			if (t == DWRITE_FONT_AXIS_TAG_OPTICAL_SIZE) {
				obs_property_set_visible(slider, !obs_data_get_bool(s, S_AUTO_OPTICAL_SIZE));
			}
		}

		src->has_variables = true;
		obs_data_set_bool(s, S_HAS_VARIABLES, true);

		obs_property_set_visible(obs_properties_get(props, S_FONT_WEIGHT), false);
		obs_property_set_visible(obs_properties_get(props, S_FONT_STRETCH), false);
	}
	else
	{
		src->has_variables = false;
		obs_data_set_bool(s, S_HAS_VARIABLES, false);
		obs_property_set_visible(variable_props, false);

		obs_property_set_visible(obs_properties_get(props, S_FONT_WEIGHT "_variable"), false);
		obs_property_set_visible(obs_properties_get(props, S_FONT_STRETCH "_variable"), false);
		obs_property_set_visible(obs_properties_get(props, S_FONT_WEIGHT), true);
		obs_property_set_visible(obs_properties_get(props, S_FONT_STRETCH), true);
	}

	return true;
#undef this
}

static bool weight_changed(void* priv, obs_properties_t* props, obs_property_t* p, obs_data_t* s)
{
	auto has_variables = obs_data_get_bool(s, S_HAS_VARIABLES);
	if (has_variables) {
		obs_data_set_int(s, S_FONT_WEIGHT, (int)ceilf(obs_data_get_double(s, S_FONT_WEIGHT "_variable") / 100) * 100);
		return false;
	}
	else {
		obs_data_set_double(s, S_FONT_WEIGHT "_variable", obs_data_get_int(s, S_FONT_WEIGHT));
		return font_changed(priv, props, p, s);;
	}
}

static bool stretch_changed(void* priv, obs_properties_t* props, obs_property_t* p, obs_data_t* s)
{
	auto has_variables = obs_data_get_bool(s, S_HAS_VARIABLES);
	if (has_variables) {
		obs_data_set_int(s, S_FONT_STRETCH, width_to_stretch(obs_data_get_double(s, S_FONT_STRETCH "_variable")));
		return false;
	}
	else {
		int stretch = obs_data_get_int(s, S_FONT_STRETCH);
		obs_data_set_double(s, S_FONT_STRETCH "_variable", stretchToWidth[(int)stretch]);
		return font_changed(priv, props, p, s);;
	}
}

static bool outline_changed(obs_properties_t* props, obs_property_t* p, obs_data_t* s)
{
	bool outline = obs_data_get_bool(s, S_OUTLINE);

	set_vis(outline, S_OUTLINE_SIZE, true);
	set_vis(outline, S_OUTLINE_COLOR, true);
	set_vis(outline, S_OUTLINE_OPACITY, true);
	return true;
}

static bool shadow_changed(obs_properties_t* props, obs_property_t* p, obs_data_t* s)
{
	bool shadow = obs_data_get_bool(s, S_SHADOW);

	set_vis(shadow, S_SHADOW_RADIUS, true);
	set_vis(shadow, S_SHADOW_COLOR, true);
	set_vis(shadow, S_SHADOW_OFFSET_X, true);
	set_vis(shadow, S_SHADOW_OFFSET_Y, true);
	set_vis(shadow, S_SHADOW_OPACITY, true);
	return true;
}

static bool chatlog_mode_changed(obs_properties_t* props, obs_property_t* p, obs_data_t* s)
{
	bool chatlog_mode = obs_data_get_bool(s, S_CHATLOG_MODE);

	set_vis(chatlog_mode, S_CHATLOG_LINES, true);
	return true;
}

static bool gradient_changed(obs_properties_t* props, obs_property_t* p, obs_data_t* s)
{
	int32_t gradient_stops = max(1, min(obs_data_get_int32(s, S_GRADIENT_EX_COUNT), MAX_GRADIENT_STOPS));
	for (size_t i = 0; i < MAX_GRADIENT_STOPS; i++) {
		set_vis(i < gradient_stops, gradient_stop_color_names[i], true);
	}

	return true;
}

static bool line_spacing_changed(obs_properties_t* props, obs_property_t* p, obs_data_t* s)
{
	int32_t spacing = obs_data_get_int(s, S_LINE_SPACING);
	bool show = spacing == S_LINE_SPACING_UNIFORM;

	set_vis(show, S_LINE_SPACING_RATIO, true);

	return true;
}

static bool extents_modified(obs_properties_t* props, obs_property_t* p, obs_data_t* s)
{
	bool use_extents = obs_data_get_bool(s, S_EXTENTS);

	set_vis(use_extents, S_EXTENTS_CX, true);
	set_vis(use_extents, S_EXTENTS_CY, true);
	return true;
}

#undef set_vis

static void upgrade_properties(void* data, obs_data_t* settings) {

	obs_data_t* font_obj = obs_data_get_obj(settings, S_FONT);
	if (!font_obj)
		return;

	do {
		HRESULT hr = S_OK;
		winrt::com_ptr<IDWriteFactory2> pDWriteFactory;
		if (FAILED(hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory2), reinterpret_cast<IUnknown**>(pDWriteFactory.put()))))
			break;

		winrt::com_ptr<IDWriteGdiInterop> pInterop;
		if (FAILED(hr = pDWriteFactory->GetGdiInterop(pInterop.put())))
			break;

		const char* font_face = obs_data_get_string(font_obj, S_FONT_FACE);
		std::wstring face = to_wide(font_face);

		LOGFONT lf = {};
		lf.lfCharSet = DEFAULT_CHARSET;
		if (!face.empty()) {
			wcscpy_s(lf.lfFaceName, face.c_str());
		}

		winrt::com_ptr<IDWriteFont> pFont;
		if (FAILED(hr = pInterop->CreateFontFromLOGFONT(&lf, pFont.put())))
			break;

		winrt::com_ptr<IDWriteFontFamily> pFontFamily;
		if (FAILED(hr = pFont->GetFontFamily(pFontFamily.put())))
			break;

		winrt::com_ptr<IDWriteLocalizedStrings> pFamilyNames;
		if (FAILED(hr = pFontFamily->GetFamilyNames(pFamilyNames.put())))
			break;

		std::wstring name{};

		UINT32 index = 0;
		UINT32 length = 0;
		BOOL exists = false;
		if (SUCCEEDED(hr = pFamilyNames->FindLocaleName(L"en-us", &index, &exists)) && exists) {
			pFamilyNames->GetStringLength(index, &length);
			name.resize(length);
			pFamilyNames->GetString(index, name.data(), length + 1);
		}

		if (!exists) {
			obs_data_set_string(settings, S_FONT_FACE, "Arial");
		}
		else {
			std::string name_s = winrt::to_string(name);
			obs_data_set_string(settings, S_FONT_FACE, name_s.data());
		}

		int64_t font_flags = obs_data_get_int(font_obj, S_FONT_FLAGS);
		if (font_flags & OBS_FONT_ITALIC) {
			obs_data_set_string(settings, S_FONT_STYLE, S_FONT_STYLE_ITALIC);
		}
		else {
			obs_data_set_string(settings, S_FONT_STYLE, S_FONT_STYLE_NONE);
		}

		obs_data_set_bool(settings, S_FONT_STRIKETHROUGH, font_flags & OBS_FONT_STRIKEOUT);
		obs_data_set_bool(settings, S_FONT_UNDERLINE, font_flags & OBS_FONT_UNDERLINE);
		obs_data_set_int(settings, S_FONT_SIZE, obs_data_get_int(font_obj, S_FONT_SIZE));

		auto weight = obs_data_get_int(settings, S_FONT_WEIGHT);
		if (weight == S_FONT_WEIGHT_AUTO) {
			obs_data_set_int(settings, S_FONT_WEIGHT, pFont->GetWeight());
		}

		auto stretch = obs_data_get_int(settings, S_FONT_WEIGHT);
		if (stretch == S_FONT_STRETCH_AUTO) {
			obs_data_set_int(settings, S_FONT_STRETCH, pFont->GetStretch());
		}

		obs_data_erase(settings, S_FONT);
	} while (false);

	obs_data_release(font_obj);
}

static obs_properties_t* get_properties(void* data)
{
	std::wstring obs_locale = to_wide(obs_get_locale());

	obs_dwrite_text_source* s = reinterpret_cast<obs_dwrite_text_source*>(data);
	if (s == nullptr)
		return nullptr;

	std::string path;

	obs_properties_t* props = obs_properties_create();
	obs_property_t* p;

	std::string filter;
	filter += T_FILTER_TEXT_FILES;
	filter += " (*.txt);;";
	filter += T_FILTER_ALL_FILES;
	filter += " (*.*)";

	if (s && !s->file.empty()) {
		const char* slash;

		path = s->file;
		replace(path.begin(), path.end(), '\\', '/');
		slash = strrchr(path.c_str(), '/');
		if (slash)
			path.resize(slash - path.c_str() + 1);
	}

	// text group
	obs_properties_t* text_group = obs_properties_create();
	obs_properties_add_group(props, S_TEXT_GROUP, T_TEXT, OBS_GROUP_NORMAL, text_group);

	obs_properties_add_text(text_group, S_TEXT, T_TEXT, OBS_TEXT_MULTILINE);
	obs_properties_add_path(text_group, S_FILE, T_FILE, OBS_PATH_FILE, filter.c_str(), path.c_str());

	p = obs_properties_add_bool(text_group, S_USE_FILE, T_USE_FILE);
	obs_property_set_modified_callback(p, use_file_changed);

	// font group
	obs_properties_t* font_group = obs_properties_create();
	obs_properties_add_group(props, S_FONT_GROUP, T_FONT, OBS_GROUP_NORMAL, font_group);

	//obs_properties_add_font(font_group, S_FONT, T_FONT);

	p = obs_properties_add_list(font_group, S_FONT_FACE, T_FONT_FACE, OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_STRING);

	std::vector<std::pair<std::string, std::string>> fonts;

	HRESULT hr = S_OK;
	do {
		UINT32 familyCount = s->pSystemFontCollection->GetFontFamilyCount();
		for (UINT32 i = 0; i < familyCount; i++) {
			winrt::com_ptr<IDWriteFontFamily> pFontFamily;
			if (FAILED(hr = s->pSystemFontCollection->GetFontFamily(i, pFontFamily.put())))
				continue;

			winrt::com_ptr<IDWriteLocalizedStrings> pFamilyNames;
			if (FAILED(hr = pFontFamily->GetFamilyNames(pFamilyNames.put())))
				continue;

			std::wstring locale_name{};
			std::wstring name{};

			UINT32 index = 0;
			UINT32 length = 0;
			BOOL exists = false;
			if (SUCCEEDED(hr = pFamilyNames->FindLocaleName(obs_locale.c_str(), &index, &exists)) && exists) {
				pFamilyNames->GetStringLength(index, &length);
				locale_name.resize(length);
				pFamilyNames->GetString(index, locale_name.data(), length + 1);
			}

			if (SUCCEEDED(hr = pFamilyNames->FindLocaleName(L"en-us", &index, &exists)) && exists) {
				pFamilyNames->GetStringLength(index, &length);
				name.resize(length);
				pFamilyNames->GetString(index, name.data(), length + 1);
			}

			if (locale_name == L"" && name == L"")
				continue;

			if (locale_name == L"")
				locale_name = name;

			std::string name_s = winrt::to_string(name), locale_name_s = winrt::to_string(locale_name);
			fonts.push_back({ name_s, locale_name_s });
		}
	} while (false);

	std::sort(fonts.begin(), fonts.end(), [](
		const std::pair<std::string, std::string>& a,
		const std::pair<std::string, std::string>& b) {
			return a.second < b.second;
		});

	for (const auto& [font, locale] : fonts) {
		obs_property_list_add_string(p, locale.c_str(), font.c_str());
	}

	obs_property_set_modified_callback2(p, font_changed, s);

	p = obs_properties_add_float(font_group, S_FONT_SIZE, T_FONT_SIZE, 1, 500, 1);
	obs_property_set_modified_callback2(p, weight_changed, s);

	p = obs_properties_add_float_slider(font_group, S_FONT_WEIGHT "_variable", T_FONT_WEIGHT, S_FONT_WEIGHT_100, S_FONT_WEIGHT_950, 0.1);
	obs_property_set_modified_callback2(p, weight_changed, s);

	p = obs_properties_add_float_slider(font_group, S_FONT_STRETCH "_variable", T_FONT_STRETCH, S_FONT_STRETCH_ULTRA_CONDENSED, S_FONT_STRETCH_ULTRA_EXPANDED, 0.1);
	obs_property_set_modified_callback2(p, stretch_changed, s);

	p = obs_properties_add_list(font_group, S_FONT_WEIGHT, T_FONT_WEIGHT, OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);
	obs_property_list_add_int(p, T_FONT_WEIGHT_100, S_FONT_WEIGHT_100);
	obs_property_list_add_int(p, T_FONT_WEIGHT_200, S_FONT_WEIGHT_200);
	obs_property_list_add_int(p, T_FONT_WEIGHT_300, S_FONT_WEIGHT_300);
	obs_property_list_add_int(p, T_FONT_WEIGHT_350, S_FONT_WEIGHT_350);
	obs_property_list_add_int(p, T_FONT_WEIGHT_400, S_FONT_WEIGHT_400);
	obs_property_list_add_int(p, T_FONT_WEIGHT_500, S_FONT_WEIGHT_500);
	obs_property_list_add_int(p, T_FONT_WEIGHT_600, S_FONT_WEIGHT_600);
	obs_property_list_add_int(p, T_FONT_WEIGHT_700, S_FONT_WEIGHT_700);
	obs_property_list_add_int(p, T_FONT_WEIGHT_800, S_FONT_WEIGHT_800);
	obs_property_list_add_int(p, T_FONT_WEIGHT_900, S_FONT_WEIGHT_900);
	obs_property_list_add_int(p, T_FONT_WEIGHT_950, S_FONT_WEIGHT_950);
	obs_property_set_modified_callback2(p, weight_changed, s);

	p = obs_properties_add_list(font_group, S_FONT_STRETCH, T_FONT_STRETCH, OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);
	obs_property_list_add_int(p, T_FONT_STRETCH_ULTRA_CONDENSED, S_FONT_STRETCH_ULTRA_CONDENSED);
	obs_property_list_add_int(p, T_FONT_STRETCH_EXTRA_CONDENSED, S_FONT_STRETCH_EXTRA_CONDENSED);
	obs_property_list_add_int(p, T_FONT_STRETCH_CONDENSED, S_FONT_STRETCH_CONDENSED);
	obs_property_list_add_int(p, T_FONT_STRETCH_SEMI_CONDENSED, S_FONT_STRETCH_SEMI_CONDENSED);
	obs_property_list_add_int(p, T_FONT_STRETCH_NORMAL, S_FONT_STRETCH_NORMAL);
	obs_property_list_add_int(p, T_FONT_STRETCH_SEMI_EXPANDED, S_FONT_STRETCH_SEMI_EXPANDED);
	obs_property_list_add_int(p, T_FONT_STRETCH_EXPANDED, S_FONT_STRETCH_EXPANDED);
	obs_property_list_add_int(p, T_FONT_STRETCH_EXTRA_EXPANDED, S_FONT_STRETCH_EXTRA_EXPANDED);
	obs_property_list_add_int(p, T_FONT_STRETCH_ULTRA_EXPANDED, S_FONT_STRETCH_ULTRA_EXPANDED);
	obs_property_set_modified_callback2(p, stretch_changed, s);

	p = obs_properties_add_list(font_group, S_FONT_STYLE, T_FONT_STYLE, OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_STRING);
	obs_property_list_add_string(p, T_FONT_STYLE_NONE, S_FONT_STYLE_NONE);
	obs_property_list_add_string(p, T_FONT_STYLE_ITALIC, S_FONT_STYLE_ITALIC);
	obs_property_list_add_string(p, T_FONT_STYLE_OBLIQUE, S_FONT_STYLE_OBLIQUE);


	p = obs_properties_add_list(font_group, S_LINE_SPACING, T_LINE_SPACING, OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);
	obs_property_list_add_int(p, T_LINE_SPACING_DEFAULT, S_LINE_SPACING_DEFAULT);
	obs_property_list_add_int(p, T_LINE_SPACING_UNIFORM, S_LINE_SPACING_UNIFORM);
	obs_property_set_modified_callback(p, line_spacing_changed);

	obs_properties_add_float(font_group, S_LINE_SPACING_RATIO, T_LINE_SPACING_RATIO, 0.01, 256, 1);

	obs_properties_add_bool(font_group, S_FONT_UNDERLINE, T_FONT_UNDERLINE);
	obs_properties_add_bool(font_group, S_FONT_STRIKETHROUGH, T_FONT_STRIKETHROUGH);

	obs_properties_t* variable_group = obs_properties_create();
	p = obs_properties_add_group(props, S_VARIABLE, T_VARIABLE, OBS_GROUP_NORMAL, variable_group);
	obs_property_set_visible(p, false);

	// colours/gradient group
	obs_properties_t* colors_group = obs_properties_create();
	obs_properties_add_group(props, S_GRADIENT_EX, T_COLOR, OBS_GROUP_NORMAL, colors_group);

	p = obs_properties_add_int_slider(colors_group, S_GRADIENT_EX_COUNT, T_COLORS, 1, MAX_GRADIENT_STOPS, 1);
	obs_property_set_modified_callback(p, gradient_changed);

	for (size_t i = 0; i < MAX_GRADIENT_STOPS; i++) {
		obs_properties_add_color(colors_group, gradient_stop_color_names[i], T_COLOR);
		//obs_properties_add_float_slider(colors, gradient_stop_offset_names[i], T_OFFSET, 0, 1, 0.01);
	}

	obs_properties_add_int_slider(colors_group, S_OPACITY, T_OPACITY, 0, 100, 1);
	obs_properties_add_float_slider(colors_group, S_GRADIENT_DIR, T_GRADIENT_DIR, 0, 360, 0.1);

	// background colour/opacity group
	obs_properties_t* background_group = obs_properties_create();
	obs_properties_add_group(props, S_BKGROUP, T_BKGROUP, OBS_GROUP_NORMAL, background_group);

	obs_properties_add_color(background_group, S_BKCOLOR, T_BKCOLOR);
	obs_properties_add_int_slider(background_group, S_BKOPACITY, T_BKOPACITY, 0, 100, 1);

	// layout group
	obs_properties_t* layout_group = obs_properties_create();
	obs_properties_add_group(props, S_LAYOUT, T_LAYOUT, OBS_GROUP_NORMAL, layout_group);

	p = obs_properties_add_list(layout_group, S_ALIGN, T_ALIGN, OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_STRING);
	obs_property_list_add_string(p, T_ALIGN_LEFT, S_ALIGN_LEFT);
	obs_property_list_add_string(p, T_ALIGN_CENTER, S_ALIGN_CENTER);
	obs_property_list_add_string(p, T_ALIGN_RIGHT, S_ALIGN_RIGHT);
	obs_property_list_add_string(p, T_ALIGN_JUSTIFIED, S_ALIGN_JUSTIFIED);

	p = obs_properties_add_list(layout_group, S_VALIGN, T_VALIGN, OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_STRING);
	obs_property_list_add_string(p, T_VALIGN_TOP, S_VALIGN_TOP);
	obs_property_list_add_string(p, T_VALIGN_CENTER, S_VALIGN_CENTER);
	obs_property_list_add_string(p, T_VALIGN_BOTTOM, S_VALIGN_BOTTOM);

	p = obs_properties_add_list(layout_group, S_WRAP_MODE, T_WRAP_MODE, OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_STRING);
	obs_property_list_add_string(p, T_WRAP_MODE_NONE, S_WRAP_MODE_NONE);
	obs_property_list_add_string(p, T_WRAP_MODE_WRAP, S_WRAP_MODE_WRAP);
	obs_property_list_add_string(p, T_WRAP_MODE_WRAP_CHARACTER, S_WRAP_MODE_WRAP_CHARACTER);
	obs_property_list_add_string(p, T_WRAP_MODE_WRAP_WHOLE_WORDS, S_WRAP_MODE_WRAP_WHOLE_WORDS);

	p = obs_properties_add_list(layout_group, S_TRIMMING, T_TRIMMING, OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);
	obs_property_list_add_int(p, T_TRIMMING_NONE, S_TRIMMING_NONE);
	obs_property_list_add_int(p, T_TRIMMING_CHARACTER_ELLIPSIS, S_TRIMMING_CHARACTER_ELLIPSIS);
	obs_property_list_add_int(p, T_TRIMMING_WORD_ELLIPSIS, S_TRIMMING_WORD_ELLIPSIS);

	p = obs_properties_add_list(layout_group, S_TRANSFORM, T_TRANSFORM, OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);
	obs_property_list_add_int(p, T_TRANSFORM_NONE, S_TRANSFORM_NONE);
	obs_property_list_add_int(p, T_TRANSFORM_UPPERCASE, S_TRANSFORM_UPPERCASE);
	obs_property_list_add_int(p, T_TRANSFORM_LOWERCASE, S_TRANSFORM_LOWERCASE);
	obs_property_list_add_int(p, T_TRANSFORM_STARTCASE, S_TRANSFORM_STARTCASE);

	p = obs_properties_add_bool(layout_group, S_EXTENTS, T_EXTENTS);
	obs_property_set_modified_callback(p, extents_modified);

	obs_properties_add_int(layout_group, S_EXTENTS_CX, T_EXTENTS_CX, -1, 8000, 1);
	obs_properties_add_int(layout_group, S_EXTENTS_CY, T_EXTENTS_CY, -1, 8000, 1);

	// shadow group
	obs_properties_t* shadow_group = obs_properties_create();
	obs_properties_add_group(props, S_SHADOW_GROUP, T_SHADOW, OBS_GROUP_NORMAL, shadow_group);

	p = obs_properties_add_bool(shadow_group, S_SHADOW, T_SHADOW);
	obs_property_set_modified_callback(p, shadow_changed);

	obs_properties_add_float(shadow_group, S_SHADOW_OFFSET_X, T_SHADOW_OFFSET_X, 0, 64, 2);
	obs_properties_add_float(shadow_group, S_SHADOW_OFFSET_Y, T_SHADOW_OFFSET_Y, 0, 64, 2);
	obs_properties_add_float(shadow_group, S_SHADOW_RADIUS, T_SHADOW_RADIUS, 0, 100, 2);
	obs_properties_add_color(shadow_group, S_SHADOW_COLOR, T_SHADOW_COLOR);
	obs_properties_add_int_slider(shadow_group, S_SHADOW_OPACITY, T_SHADOW_OPACITY, 0, 100, 1);

	// outline group
	obs_properties_t* outline_group = obs_properties_create();
	obs_properties_add_group(props, S_OUTLINE_GROUP, T_OUTLINE, OBS_GROUP_NORMAL, outline_group);

	p = obs_properties_add_bool(outline_group, S_OUTLINE, T_OUTLINE);
	obs_property_set_modified_callback(p, outline_changed);

	obs_properties_add_int(outline_group, S_OUTLINE_SIZE, T_OUTLINE_SIZE, 1, 20, 1);
	obs_properties_add_color(outline_group, S_OUTLINE_COLOR, T_OUTLINE_COLOR);
	obs_properties_add_int_slider(outline_group, S_OUTLINE_OPACITY, T_OUTLINE_OPACITY, 0, 100, 1);

	// Font features group
	obs_properties_t* feature_group = obs_properties_create();
	obs_properties_add_group(props, S_FONT_FEATURES, T_FONT_FEATURES, OBS_GROUP_NORMAL, feature_group);

	// Add a boolean property for every DWRITE_FONT_FEATURE_TAG member
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_ALTERNATIVE_FRACTIONS, T_FONT_FEATURE_ALTERNATIVE_FRACTIONS);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_PETITE_CAPITALS_FROM_CAPITALS, T_FONT_FEATURE_PETITE_CAPITALS_FROM_CAPITALS);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_SMALL_CAPITALS_FROM_CAPITALS, T_FONT_FEATURE_SMALL_CAPITALS_FROM_CAPITALS);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_CONTEXTUAL_ALTERNATES, T_FONT_FEATURE_CONTEXTUAL_ALTERNATES);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_CASE_SENSITIVE_FORMS, T_FONT_FEATURE_CASE_SENSITIVE_FORMS);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_GLYPH_COMPOSITION_DECOMPOSITION, T_FONT_FEATURE_GLYPH_COMPOSITION_DECOMPOSITION);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_CONTEXTUAL_LIGATURES, T_FONT_FEATURE_CONTEXTUAL_LIGATURES);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_CAPITAL_SPACING, T_FONT_FEATURE_CAPITAL_SPACING);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_CONTEXTUAL_SWASH, T_FONT_FEATURE_CONTEXTUAL_SWASH);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_CURSIVE_POSITIONING, T_FONT_FEATURE_CURSIVE_POSITIONING);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_DEFAULT, T_FONT_FEATURE_DEFAULT);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_DISCRETIONARY_LIGATURES, T_FONT_FEATURE_DISCRETIONARY_LIGATURES);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_EXPERT_FORMS, T_FONT_FEATURE_EXPERT_FORMS);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_FRACTIONS, T_FONT_FEATURE_FRACTIONS);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_FULL_WIDTH, T_FONT_FEATURE_FULL_WIDTH);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_HALF_FORMS, T_FONT_FEATURE_HALF_FORMS);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_HALANT_FORMS, T_FONT_FEATURE_HALANT_FORMS);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_ALTERNATE_HALF_WIDTH, T_FONT_FEATURE_ALTERNATE_HALF_WIDTH);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_HISTORICAL_FORMS, T_FONT_FEATURE_HISTORICAL_FORMS);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_HORIZONTAL_KANA_ALTERNATES, T_FONT_FEATURE_HORIZONTAL_KANA_ALTERNATES);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_HISTORICAL_LIGATURES, T_FONT_FEATURE_HISTORICAL_LIGATURES);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_HALF_WIDTH, T_FONT_FEATURE_HALF_WIDTH);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_HOJO_KANJI_FORMS, T_FONT_FEATURE_HOJO_KANJI_FORMS);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_JIS04_FORMS, T_FONT_FEATURE_JIS04_FORMS);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_JIS78_FORMS, T_FONT_FEATURE_JIS78_FORMS);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_JIS83_FORMS, T_FONT_FEATURE_JIS83_FORMS);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_JIS90_FORMS, T_FONT_FEATURE_JIS90_FORMS);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_KERNING, T_FONT_FEATURE_KERNING);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_STANDARD_LIGATURES, T_FONT_FEATURE_STANDARD_LIGATURES);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_LINING_FIGURES, T_FONT_FEATURE_LINING_FIGURES);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_LOCALIZED_FORMS, T_FONT_FEATURE_LOCALIZED_FORMS);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_MARK_POSITIONING, T_FONT_FEATURE_MARK_POSITIONING);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_MATHEMATICAL_GREEK, T_FONT_FEATURE_MATHEMATICAL_GREEK);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_MARK_TO_MARK_POSITIONING, T_FONT_FEATURE_MARK_TO_MARK_POSITIONING);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_ALTERNATE_ANNOTATION_FORMS, T_FONT_FEATURE_ALTERNATE_ANNOTATION_FORMS);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_NLC_KANJI_FORMS, T_FONT_FEATURE_NLC_KANJI_FORMS);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_OLD_STYLE_FIGURES, T_FONT_FEATURE_OLD_STYLE_FIGURES);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_ORDINALS, T_FONT_FEATURE_ORDINALS);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_PROPORTIONAL_ALTERNATE_WIDTH, T_FONT_FEATURE_PROPORTIONAL_ALTERNATE_WIDTH);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_PETITE_CAPITALS, T_FONT_FEATURE_PETITE_CAPITALS);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_PROPORTIONAL_FIGURES, T_FONT_FEATURE_PROPORTIONAL_FIGURES);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_PROPORTIONAL_WIDTHS, T_FONT_FEATURE_PROPORTIONAL_WIDTHS);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_QUARTER_WIDTHS, T_FONT_FEATURE_QUARTER_WIDTHS);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_REQUIRED_LIGATURES, T_FONT_FEATURE_REQUIRED_LIGATURES);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_RUBY_NOTATION_FORMS, T_FONT_FEATURE_RUBY_NOTATION_FORMS);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_STYLISTIC_ALTERNATES, T_FONT_FEATURE_STYLISTIC_ALTERNATES);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_SCIENTIFIC_INFERIORS, T_FONT_FEATURE_SCIENTIFIC_INFERIORS);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_SMALL_CAPITALS, T_FONT_FEATURE_SMALL_CAPITALS);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_SIMPLIFIED_FORMS, T_FONT_FEATURE_SIMPLIFIED_FORMS);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_STYLISTIC_SET_1, T_FONT_FEATURE_STYLISTIC_SET_1);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_STYLISTIC_SET_2, T_FONT_FEATURE_STYLISTIC_SET_2);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_STYLISTIC_SET_3, T_FONT_FEATURE_STYLISTIC_SET_3);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_STYLISTIC_SET_4, T_FONT_FEATURE_STYLISTIC_SET_4);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_STYLISTIC_SET_5, T_FONT_FEATURE_STYLISTIC_SET_5);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_STYLISTIC_SET_6, T_FONT_FEATURE_STYLISTIC_SET_6);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_STYLISTIC_SET_7, T_FONT_FEATURE_STYLISTIC_SET_7);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_STYLISTIC_SET_8, T_FONT_FEATURE_STYLISTIC_SET_8);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_STYLISTIC_SET_9, T_FONT_FEATURE_STYLISTIC_SET_9);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_STYLISTIC_SET_10, T_FONT_FEATURE_STYLISTIC_SET_10);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_STYLISTIC_SET_11, T_FONT_FEATURE_STYLISTIC_SET_11);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_STYLISTIC_SET_12, T_FONT_FEATURE_STYLISTIC_SET_12);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_STYLISTIC_SET_13, T_FONT_FEATURE_STYLISTIC_SET_13);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_STYLISTIC_SET_14, T_FONT_FEATURE_STYLISTIC_SET_14);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_STYLISTIC_SET_15, T_FONT_FEATURE_STYLISTIC_SET_15);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_STYLISTIC_SET_16, T_FONT_FEATURE_STYLISTIC_SET_16);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_STYLISTIC_SET_17, T_FONT_FEATURE_STYLISTIC_SET_17);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_STYLISTIC_SET_18, T_FONT_FEATURE_STYLISTIC_SET_18);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_STYLISTIC_SET_19, T_FONT_FEATURE_STYLISTIC_SET_19);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_STYLISTIC_SET_20, T_FONT_FEATURE_STYLISTIC_SET_20);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_SUBSCRIPT, T_FONT_FEATURE_SUBSCRIPT);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_SUPERSCRIPT, T_FONT_FEATURE_SUPERSCRIPT);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_SWASH, T_FONT_FEATURE_SWASH);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_TITLING, T_FONT_FEATURE_TITLING);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_TRADITIONAL_NAME_FORMS, T_FONT_FEATURE_TRADITIONAL_NAME_FORMS);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_TABULAR_FIGURES, T_FONT_FEATURE_TABULAR_FIGURES);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_TRADITIONAL_FORMS, T_FONT_FEATURE_TRADITIONAL_FORMS);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_THIRD_WIDTHS, T_FONT_FEATURE_THIRD_WIDTHS);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_UNICASE, T_FONT_FEATURE_UNICASE);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_VERTICAL_WRITING, T_FONT_FEATURE_VERTICAL_WRITING);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_VERTICAL_ALTERNATES_AND_ROTATION, T_FONT_FEATURE_VERTICAL_ALTERNATES_AND_ROTATION);
	obs_properties_add_bool(feature_group, S_FONT_FEATURE_SLASHED_ZERO, T_FONT_FEATURE_SLASHED_ZERO);

	// advanced group
	obs_properties_t* advanced_group = obs_properties_create();
	obs_properties_add_group(props, S_ADVANCED, T_ADVANCED, OBS_GROUP_NORMAL, advanced_group);

	p = obs_properties_add_bool(advanced_group, S_CHATLOG_MODE, T_CHATLOG_MODE);
	obs_property_set_modified_callback(p, chatlog_mode_changed);

	obs_properties_add_int(advanced_group, S_CHATLOG_LINES, T_CHATLOG_LINES, 1, 1000, 1);

	p = obs_properties_add_bool(advanced_group, S_ANTIALIASING, T_ANTIALIASING);
	p = obs_properties_add_bool(advanced_group, S_COLOR_FONTS, T_COLOR_FONTS);

	return props;
}

bool obs_module_load(void)
{
	obs_source_info si = {};
	si.id = "text_directwrite";
	si.type = OBS_SOURCE_TYPE_INPUT;
	si.icon_type = OBS_ICON_TYPE_TEXT;
	si.get_properties = get_properties;
	si.output_flags = OBS_SOURCE_VIDEO | OBS_SOURCE_CUSTOM_DRAW | OBS_SOURCE_SRGB;

	si.get_name = [](void*) { return obs_module_text("TextDirectWrite"); };
	si.create = [](obs_data_t* settings, obs_source_t* source) {
		return (void*)new obs_dwrite_text_source(source, settings);
		};
	si.destroy = [](void* data) { delete reinterpret_cast<obs_dwrite_text_source*>(data); };
	si.get_width = [](void* data) { return reinterpret_cast<obs_dwrite_text_source*>(data)->get_width(); };
	si.get_height = [](void* data) {
		return reinterpret_cast<obs_dwrite_text_source*>(data)->get_height();
		};
	si.get_defaults = [](obs_data_t* settings) {
		obs_data_set_default_string(settings, S_FONT_FACE, "Arial");
		obs_data_set_default_int(settings, S_FONT_SIZE, 256);
		obs_data_set_default_string(settings, S_FONT_STYLE, S_FONT_STYLE_NONE);
		obs_data_set_default_string(settings, S_ALIGN, S_ALIGN_LEFT);
		obs_data_set_default_string(settings, S_VALIGN, S_VALIGN_TOP);
		obs_data_set_default_string(settings, S_WRAP_MODE, S_WRAP_MODE_WRAP);
		obs_data_set_default_int(settings, S_FONT_WEIGHT, S_FONT_WEIGHT_400);
		obs_data_set_default_int(settings, S_FONT_STRETCH, S_FONT_STRETCH_NORMAL);
		obs_data_set_default_int(settings, S_COLOR, 0xFFFFFF);
		obs_data_set_default_int(settings, S_GRADIENT_EX_COUNT, 1);
		obs_data_set_default_int(settings, S_OPACITY, 100);
		obs_data_set_default_double(settings, S_GRADIENT_DIR, 90.0);
		obs_data_set_default_int(settings, S_BKCOLOR, 0x000000);
		obs_data_set_default_int(settings, S_BKOPACITY, 0);
		obs_data_set_default_int(settings, S_OUTLINE_SIZE, 2);
		obs_data_set_default_int(settings, S_OUTLINE_COLOR, 0xFFFFFF);
		obs_data_set_default_int(settings, S_OUTLINE_OPACITY, 100);
		obs_data_set_default_int(settings, S_CHATLOG_LINES, 6);
		obs_data_set_default_int(settings, S_SHADOW_RADIUS, 4);
		obs_data_set_default_int(settings, S_SHADOW_COLOR, 0x000000);
		obs_data_set_default_int(settings, S_SHADOW_OPACITY, 50);
		obs_data_set_default_int(settings, S_EXTENTS_CX, 100);
		obs_data_set_default_int(settings, S_EXTENTS_CY, 100);
		obs_data_set_default_bool(settings, S_COLOR_FONTS, true);
		obs_data_set_default_bool(settings, S_ANTIALIASING, true);

		obs_data_set_default_bool(settings, S_FONT_FEATURE_KERNING, true);
		obs_data_set_default_bool(settings, S_FONT_FEATURE_GLYPH_COMPOSITION_DECOMPOSITION, true);
		obs_data_set_default_bool(settings, S_FONT_FEATURE_REQUIRED_LIGATURES, true);
		obs_data_set_default_bool(settings, S_FONT_FEATURE_STANDARD_LIGATURES, true);
		obs_data_set_default_bool(settings, S_FONT_FEATURE_CONTEXTUAL_LIGATURES, true);
		obs_data_set_default_bool(settings, S_FONT_FEATURE_CONTEXTUAL_ALTERNATES, true);
		obs_data_set_default_bool(settings, S_FONT_FEATURE_CURSIVE_POSITIONING, true);
		obs_data_set_default_bool(settings, S_FONT_FEATURE_MARK_POSITIONING, true);
		obs_data_set_default_bool(settings, S_FONT_FEATURE_MARK_TO_MARK_POSITIONING, true);
		obs_data_set_default_bool(settings, S_FONT_FEATURE_LOCALIZED_FORMS, true);

		obs_data_set_default_bool(settings, S_AUTO_OPTICAL_SIZE, true);
		};
	si.update = [](void* data, obs_data_t* settings) {
		reinterpret_cast<obs_dwrite_text_source*>(data)->Update(settings);
		};
	si.video_tick = [](void* data, float seconds) {
		reinterpret_cast<obs_dwrite_text_source*>(data)->Tick(seconds);
		};
	si.video_render = [](void* data, gs_effect_t* effect) {
		reinterpret_cast<obs_dwrite_text_source*>(data)->Render(effect);
		};


	obs_register_source(&si);

	return true;
}

void obs_module_unload(void) {}
