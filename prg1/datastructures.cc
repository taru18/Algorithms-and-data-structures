// Datastructures.cc

#include "datastructures.hh"
#include <random>
#include <cmath>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <memory>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{}

Datastructures::~Datastructures()
{}

int Datastructures::beacon_count()
{
    return allBeacons.size();
}

void Datastructures::clear_beacons()
{
    allBeacons.clear();
    beaconNames.clear();
    beaconBrightnesses.clear();
}

std::vector<BeaconID> Datastructures::all_beacons()
{
    std::vector<BeaconID> beacons = {};
    for (auto beacon : allBeacons){
        beacons.push_back(beacon.first);
    }
    return beacons;
}

void Datastructures::update_brightness(BeaconID id)
{
    Color color = allBeacons.at(id).color;
    allBeacons.at(id).brightness = 3*color.r+6*color.g+color.b;
}

bool Datastructures::add_beacon(BeaconID newId, const std::string& newName, Coord xy, Color newColor)
{
    if (allBeacons.find(newId) != allBeacons.end()){
        return false;
    }
    Beacon b = Beacon{newId, xy, newName, newColor, nullptr, {}};
    allBeacons[newId] = b;
    update_brightness(newId);
    beaconNames.insert({newName, newId});
    beaconBrightnesses.insert({allBeacons.at(newId).brightness, newId});
    return true;
}

std::string Datastructures::get_name(BeaconID id)
{
    if (allBeacons.find(id) != allBeacons.end()) {
        return allBeacons.at(id).name;
    }
    return NO_NAME;
}

Coord Datastructures::get_coordinates(BeaconID id)
{
    if (allBeacons.find(id) != allBeacons.end()) {
        return allBeacons.at(id).coord;
    }
    return NO_COORD;
}

Color Datastructures::get_color(BeaconID id)
{
    if (allBeacons.find(id) != allBeacons.end()) {
        return allBeacons.at(id).color;
    }
    return NO_COLOR;
}

//Palauttaa majakkojen id:t majakoiden nimen mukaan aakkosjärjestyksessä
std::vector<BeaconID> Datastructures::beacons_alphabetically()
{
    std::vector<BeaconID> ids = {};
    if (!beaconNames.empty()) {
        for (auto name : beaconNames) {
            ids.push_back(name.second);
        }
    }
    return ids;
}

//Palauttaa majakkojen id:t majakoiden oman värin kirkkausjärjestyksessä, himmein ensin.
std::vector<BeaconID> Datastructures::beacons_brightness_increasing()
{
    std::vector<BeaconID> ids = {};
    if (!beaconBrightnesses.empty()) {
        for (auto id : beaconBrightnesses) {
            ids.push_back(id.second);
        }
    }
    return ids;
}

//Palauttaa majakan, jonka oma väri on himmein. Jos tällaisia on useita, palauttaa jonkin niistä.
//Jos majakoita ei ole, palautetaan NO_ID.
BeaconID Datastructures::min_brightness()
{
    BeaconID minId = NO_ID;
    if (!beaconBrightnesses.empty()) {
        minId = beaconBrightnesses.begin()->second;
    }
    return minId;
}

//Palauttaa majakan, jonka oma väri on kirkkain. Jos tällaisia on useita, palauttaa jonkin niistä.
//Jos majakoita ei ole, palautetaan NO_ID.
BeaconID Datastructures::max_brightness()
{
    BeaconID maxId = NO_ID;
    if (!beaconBrightnesses.empty()) {
        maxId = beaconBrightnesses.rbegin()->second;
    }
    return maxId;
}

//Palauttaa majakat, joilla on annettu nimi tai tyhjän vektorin, jos sellaisia ei ole.
//Paluuarvo on järjestettävä nousevan ID:n mukaiseen järjestykseen.
std::vector<BeaconID> Datastructures::find_beacons(std::string const& name)
{
    std::vector<BeaconID> beacons = {};
    for (auto beacon : beaconNames) {
        if (beacon.first == name) {
            beacons.push_back(beacon.second);
        }
    }
    std::sort(beacons.begin(), beacons.end());
    return beacons;
}

//Muuttaa annetulla ID:llä olevan majakan nimen. Jos majakkaa ei löydy, palauttaa false, muuten true.
bool Datastructures::change_beacon_name(BeaconID id, const std::string& newname)
{
    if (allBeacons.find(id) != allBeacons.end()) {
        std::string oldName = allBeacons.at(id).name;
        allBeacons[id].name = newname;

        auto iterpair = beaconNames.equal_range(oldName);
        auto it = iterpair.first;
        for (; it != iterpair.second; ++it) {
            if (it->second == id) {
                beaconNames.erase(it);
                break;
            }
        }
        beaconNames.insert({newname,id});
        return true;
    }
    return false;
}

//Muuttaa annetulla ID:llä olevan majakan värin. Jos majakkaa ei löydy, palauttaa false, muuten true.
bool Datastructures::change_beacon_color(BeaconID id, Color newcolor)
{
    if (allBeacons.find(id) != allBeacons.end()) {
        int oldBrightness = allBeacons.at(id).brightness;
        allBeacons[id].color = newcolor;
        update_brightness(id);

        auto iterpair = beaconBrightnesses.equal_range(oldBrightness);
        auto it = iterpair.first;
        for (; it != iterpair.second; ++it) {
            if (it->second == id) {
                beaconBrightnesses.erase(it);
                break;
            }
        }
        beaconBrightnesses.insert({allBeacons.at(id).brightness, id});
        return true;
    }
    return false;
}

//Lisää valonsäteen majakasta toiseen. Majakka voi lähettää valoa vain yhdelle toiselle majakalle.
//Työssä saa olettaa, että valonsäteet eivät voi muodostaa silmukoita (ts. majakka ei voi suoraan tai epäsuorasti valaista itseään).
//Jos jompaa kumpaa majakkaa ei löydy tai lähdemajakka lähettää jo valoa toiselle majakalle, ei tehdä mitään ja palautetaan false. Muuten palautetaan true.
bool Datastructures::add_lightbeam(BeaconID sourceid, BeaconID targetid)
{
    if (allBeacons.find(sourceid) != allBeacons.end() and allBeacons.find(targetid) != allBeacons.end() and allBeacons.at(sourceid).sending == nullptr) {
        allBeacons.at(targetid).receiving.push_back(std::make_shared<Beacon>(allBeacons.at(sourceid)));
        allBeacons.at(sourceid).sending = std::make_shared<Beacon>(allBeacons.at(targetid));
        return true;
    }
    return false;
}

//Palauttaa niiden majakoiden id:t, jotka lähettävät valonsa suoraan annetulla id:llä olevalle majakalle, tai vektorin jonka ainoa alkio on NO_ID,
//jos id:llä ei löydy majakkaa. Paluuarvo on järjestettävä nousevan ID:n mukaiseen järjestykseen.
std::vector<BeaconID> Datastructures::get_lightsources(BeaconID id)
{
    std::vector<BeaconID> ids;
    if (allBeacons.find(id) != allBeacons.end()) {
        std::vector<std::shared_ptr<Beacon>> beams = allBeacons.at(id).receiving;
        for (auto beam :  beams) {
            ids.push_back(beam->id);
        }
        std::sort(ids.begin(), ids.end());
        return ids;
    }
    return {{NO_ID}};
}

std::vector<BeaconID> Datastructures::path_recursive(BeaconID id)
{
    std::vector<BeaconID> outbeams;
    if (allBeacons.at(id).sending != nullptr) {
        outbeams = path_recursive(allBeacons.at(id).sending->id);
    }
    outbeams.push_back(id);
    return outbeams;
}

//Palauttaa listan majakoista, joille annettu majakka välittää valoa suoraan tai epäsuorasti.
//Paluuvektoriin talletetaan ensin majakka itse, sitten sen valon suora kohdemajakka, kohdemajakan kohdemajakka jne.
//niin kauan kuin valonsäteitä riittää. Jos id:llä ei ole majakkaa, palautetaan vektori, jonka ainoa alkio on NO_ID.
std::vector<BeaconID> Datastructures::path_outbeam(BeaconID id)
{
    if (allBeacons.find(id) == allBeacons.end()) {
        return {{NO_ID}};
    }
    std::vector<BeaconID> beams = path_recursive(id);
    std::reverse(beams.begin(), beams.end());
    return beams;
}

//Poistaa annetulla id:llä olevan majakan. Jos id ei vastaa mitään majakkaa, ei tehdä mitään ja palautetaan false, muuten palautetaan true.
//Jos poistettavasta majakasta tulee tai lähtee valonsäteitä, nämä valonsäteet poistetaan.
bool Datastructures::remove_beacon(BeaconID id)
{
    if (allBeacons.find(id) == allBeacons.end()) {
        return false;
    }
    auto iterpair = beaconBrightnesses.equal_range(allBeacons.at(id).brightness);
    auto it = iterpair.first;
    for (; it != iterpair.second; ++it) {
        if (it->second == id) {
            beaconBrightnesses.erase(it);
            break;
        }
    }
    auto iterpair2 = beaconNames.equal_range(allBeacons.at(id).name);
    auto it2 = iterpair2.first;
    for (; it2 != iterpair2.second; ++it2) {
        if (it2->second == id) {
            beaconNames.erase(it2);
            break;
        }
    }
    if (allBeacons.at(id).sending != nullptr) {
        std::shared_ptr<Beacon> target = allBeacons.at(id).sending;
        std::shared_ptr<Beacon> source = std::make_shared<Beacon>(allBeacons.at(id));
        allBeacons.at(target->id).receiving.erase(std::remove(allBeacons.at(target->id).receiving.begin(),
                                                              allBeacons.at(target->id).receiving.end(), source));
    }
    if (!allBeacons.at(id).receiving.empty()) {
        for (auto beacon : allBeacons.at(id).receiving) {
            allBeacons.at(beacon->id).sending = nullptr;
        }
    }
    allBeacons.erase(id);
    return true;
}

std::vector<BeaconID> Datastructures::path_inbeam_longest(BeaconID /*id*/)
{
    return {{NO_ID}};
}

Color Datastructures::color_recursive(BeaconID id)
{
    Color thisColor = allBeacons.at(id).color;

    if (allBeacons.at(id).receiving.empty()) {
        return allBeacons.at(id).color;
    }
    for (auto beacon : allBeacons.at(id).receiving) {
            Color someColor = color_recursive(beacon->id);
            thisColor.r += someColor.r;
            thisColor.g += someColor.g;
            thisColor.b += someColor.b;
    }
    int divider = static_cast<int>(allBeacons.at(id).receiving.size()+1);
    int newR = thisColor.r / divider;
    int newG = thisColor.g / divider;
    int newB = thisColor.b / divider;

    return {newR, newG, newB};
}

//Palauttaa majakasta lähtevän valon kokonaisvärin eli tulevien säteiden ja majakan oman värin keskiarvon.
//Jos id:lläei löydy majakkaa, palautetaan NO_COLOR.
Color Datastructures::total_color(BeaconID id)
{
    if (allBeacons.find(id) == allBeacons.end()) {
        return NO_COLOR;
    }
    return color_recursive(id);
}
