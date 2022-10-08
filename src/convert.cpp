#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tinyxml2.h>
#include <regex>
#include <string>
#include <vector>

std::vector<std::string> tokenize(const std::string& s, char c) {
    auto end = s.cend();
    auto start = end;

    std::vector<std::string> v;
    for( auto it = s.cbegin(); it != end; ++it ) {
        if( *it != c ) {
            if( start == end )
                start = it;
            continue;
        }
        if( start != end ) {
            v.emplace_back(start, it);
            start = end;
        }
    }
    if( start != end )
        v.emplace_back(start, end);
    return v;
}

char hex(int x) {
    return x > 9 ? x + 'A' - 10 : x + '0';
}

std::string hexstr(int x) {
    char c[3];
    c[0] = hex(x/16);
    c[1] = hex(x%16);
    c[2] = '\0';
    return c;
}

int test(int argc, char* argv[]) {
    tinyxml2::XMLDocument xmlDoc;
    if (argc < 2) {
        return 1;
    }
    auto err = xmlDoc.LoadFile(argv[1]);
    if (err != tinyxml2::XML_SUCCESS) {
        return 1;
    }
    auto root = xmlDoc.FirstChild();
    auto outRoot = xmlDoc.NewElement("flames");
    auto palette = xmlDoc.NewElement("palette");
    xmlDoc.InsertFirstChild(outRoot);
    auto header = root->ToElement();
    header->SetAttribute("version", "EMBER-LNX-22.21.4.2");
    int quality = header->IntAttribute("quality");
    header->SetAttribute("quality", quality*1000);
    header->DeleteAttribute("passes");
    int temporalSamples = header->IntAttribute("temporal_samples");
    header->SetAttribute("temporal_samples", temporalSamples*100);
    header->SetAttribute("sub_batch_size", 10240);
    header->SetAttribute("rand_range", 1);
    header->SetAttribute("logscale_k2", 0);
    header->SetAttribute("cam_zpos", 0);
    header->SetAttribute("cam_persp", 0);
    header->SetAttribute("cam_yaw", 0);
    header->SetAttribute("cam_pitch", 0);
    header->SetAttribute("cam_dof", 0);
    header->SetAttribute("blur_curve", 0);
    header->SetAttribute("interpolation", "linear");
    header->DeleteAttribute("palette_interpolation");
    header->SetAttribute("plugins", "linear spherical flatten");
    header->SetAttribute("new_linear", 1);
    header->SetAttribute("curves", "0 0 1 0.25 0.25 1 0.5 0.5 1 0.75 0.75 1 "
            "0 0 1 0.25 0.25 1 0.5 0.5 1 0.75 0.75 1 0 0 1 0.25 0.25 1 0.5 "
            "0.5 1 0.75 0.75 1 0 0 1 0.25 0.25 1 0.5 0.5 1 0.75 0.75 1 ");
    char curveStr[] = "0 0 0.25 0.25 0.5 0.5 0.75 0.75 1 1 ";
    header->SetAttribute("overall_curve", curveStr);
    header->SetAttribute("red_curve", curveStr);
    header->SetAttribute("green_curve", curveStr);
    header->SetAttribute("blue_curve", curveStr);
    auto node = header->FirstChild();
    std::vector<tinyxml2::XMLElement*> forms;
    for (; strcmp(node->Value(),"xform") == 0; node = node->NextSibling()) {
        forms.push_back(node->ShallowClone(&xmlDoc)->ToElement());
    }
    std::string text = "\n";
    int npal = 0;
    for (; node != NULL; node = node->NextSibling()) {
        std::string rgb = node->ToElement()->Attribute("rgb");
        auto channels = tokenize(rgb, ' ');
        int r = atoi(channels[0].c_str());
        int g = atoi(channels[1].c_str());
        int b = atoi(channels[2].c_str());
        text += hexstr(r);
        text += hexstr(g);
        text += hexstr(b);
        npal++;
        if (npal % 8 == 0) {
            text += "\n";
        }
    }
    palette->SetAttribute("count", 256);
    palette->SetAttribute("format", "RGB");
    palette->SetText(text.c_str());
    for (int i=0; i<forms.size()+npal; i++) {
        header->DeleteChild(header->FirstChild());
    }
    for (int i=0; i<forms.size(); i++) {
        forms[i]->SetAttribute("var_color", 1);
        forms[i]->SetAttribute("symmetry", 0);
        forms[i]->SetAttribute("name", "");
        forms[i]->SetAttribute("flatten", 1);
        const char* chaos = forms[i]->Attribute("chaos");
        std::string s = (chaos == NULL) ? "" : chaos;
        int ntok = tokenize(s, ' ').size();
        for (int j=0; j<forms.size()-ntok; j++) {
            s += "1 ";
        }
        forms[i]->SetAttribute("chaos", s.c_str());
        header->InsertFirstChild(forms[i]);
    }
    outRoot->InsertEndChild(header);
    header->InsertEndChild(palette);
    xmlDoc.SaveFile("test.xml");
    return 0;
}
