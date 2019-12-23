// Datastructures.cc

#include "datastructures.hh"
#include <random>
#include <cmath>
#include <algorithm>
#include <memory>
#include <iterator>
#include <list>
#include <unordered_map>
#include <QDebug>

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

BeaconID Datastructures::min_brightness()
{
    BeaconID minId = NO_ID;
    if (!beaconBrightnesses.empty()) {
        minId = beaconBrightnesses.begin()->second;
    }
    return minId;
}

BeaconID Datastructures::max_brightness()
{
    BeaconID maxId = NO_ID;
    if (!beaconBrightnesses.empty()) {
        maxId = beaconBrightnesses.rbegin()->second;
    }
    return maxId;
}

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

bool Datastructures::add_lightbeam(BeaconID sourceid, BeaconID targetid)
{
    if (allBeacons.find(sourceid) != allBeacons.end() and allBeacons.find(targetid) != allBeacons.end() and allBeacons.at(sourceid).sending == nullptr) {
        allBeacons.at(targetid).receiving.push_back(std::make_shared<Beacon>(allBeacons.at(sourceid)));
        allBeacons.at(sourceid).sending = std::make_shared<Beacon>(allBeacons.at(targetid));
        return true;
    }
    return false;
}

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

std::vector<BeaconID> Datastructures::path_outbeam(BeaconID id)
{
    if (allBeacons.find(id) == allBeacons.end()) {
        return {{NO_ID}};
    }
    std::vector<BeaconID> beams = path_recursive(id);
    std::reverse(beams.begin(), beams.end());
    return beams;
}

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
    return {NO_ID};
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

Color Datastructures::total_color(BeaconID id)
{
    if (allBeacons.find(id) == allBeacons.end()) {
        return NO_COLOR;
    }
    return color_recursive(id);
}

std::pair<Coord, Coord> Datastructures::swapCoords(std::pair<Coord, Coord> point)
{
    if (operator<(point.second, point.first)) {
        std::swap(point.first, point.second);
    }
    return point;
}

// Palauttaa kaikki tietorakenteessa olevat kuitujen päätepisteet koordinaattien mukaisessa järjestyksessä
// ja jokainen päätepiste on mukana vain kerran.
std::vector<Coord> Datastructures::all_xpoints()
{
    std::vector<Coord> xpoints = {};
    for (auto x : allFibres) {
        if (std::find(xpoints.begin(), xpoints.end(), x.first) == xpoints.end()) {
            xpoints.push_back(x.first);
        }
    }
    return xpoints;
}

// Lisää tietorakenteeseen uuden valokuidun annettujen pisteiden välille ja annetulla hinnalla.
// Jos annettujen pisteiden välillä on jo kuitu tai jos molemmat pisteet ovat samat, ei tehdä mitään ja palautetaan false, muuten palautetaan true
bool Datastructures::add_fibre(Coord xpoint1, Coord xpoint2, Cost cost)
{
    std::pair<Coord, Coord> points = swapCoords(std::make_pair(xpoint1, xpoint2));

    if (xpoint1 == xpoint2 or fibreCoords.find(points) != fibreCoords.end()) {
        return false;
    }

    if (allFibres.find(points.first) == allFibres.end()) {
        Xpoint f = Xpoint{points.first, {{points.second, cost}}};
        allFibres[points.first] = f;
    }
    else {
        allFibres[points.first].fibres.insert({points.second, cost});
    }

    if (allFibres.find(points.second) == allFibres.end()) {
        Xpoint fi = Xpoint{points.second, {{points.first, cost}}};
        allFibres[points.second] = fi;
    }
    else {
        allFibres[points.second].fibres.insert({points.first, cost});
    }

    fibreCoords[points] = cost;
    return true;
}

// Palauttaa listan koordinaatteja, joihin annetusta koordinaatista menee suoraan kuituja, ja ko. kuidun hinnan.
// Lista on järjestetty koordinaattien mukaiseen järjestykseen. Jos annetusta pisteestä ei lähde kuituja, palautetaan tyhjä lista.
std::vector<std::pair<Coord, Cost>> Datastructures::get_fibres_from(Coord xpoint)
{
    std::vector<std::pair<Coord, Cost>> coords = {};

    if (allFibres.find(xpoint) != allFibres.end()) {

        std::for_each(allFibres.at(xpoint).fibres.begin(), allFibres.at(xpoint).fibres.end(),
                      [&](const std::pair<Coord, Cost> &p){coords.push_back(p);});

    }
    return coords;
}

// Palauttaa listan kaikista kuiduista. Listassa jokainen kuitu ilmoitetaan sen päätepisteiden muodostamana koordinaattiparina niin,
// että lista on järjestetty ensisijaisesti ensimmäisen koordinaatin mukaan, toissijaisesti toisen.
std::vector<std::pair<Coord, Coord>> Datastructures::all_fibres()
{
    std::vector<std::pair<Coord, Coord>> fibres = {};

    for (auto pair : fibreCoords) {
        fibres.push_back(pair.first);
    }
    return fibres;
}

// Poistaa kuidun annettujen koordinaattien väliltä. Jos koordinaattien välillä ei ollut kuitua,
// palautetaan false, muuten true.
bool Datastructures::remove_fibre(Coord xpoint1, Coord xpoint2)
{    
    std::pair<Coord, Coord> points = swapCoords(std::make_pair(xpoint1, xpoint2));

    if (fibreCoords.find(points) == fibreCoords.end() ) {
        return false;
    }

    fibreCoords.erase(points);

    if (allFibres.find(xpoint1) != allFibres.end()) {
        allFibres.at(xpoint1).fibres.erase(xpoint2);
        if (allFibres.at(xpoint1).fibres.empty()) {
            allFibres.erase(xpoint1);
        }
    }

    if (allFibres.find(xpoint2) != allFibres.end()) {
        allFibres.at(xpoint2).fibres.erase(xpoint1);
        if (allFibres.at(xpoint2).fibres.empty()) {
            allFibres.erase(xpoint2);
        }
    }

    return true;
}

void Datastructures::clear_fibres()
{
    allFibres.clear();
    fibreCoords.clear();
}

std::vector<Coord> Datastructures::bfsRoute(Coord from, Coord to)
{
    std::list<std::vector<Coord>> queue;
    std::vector<Coord> route;
    route.push_back(from);
    queue.push_back(route);

    while (!queue.empty()) {
        route = queue.front();
        queue.pop_front();
        Coord last = route[route.size() -1];

        if (last == to) {
            return route;
        }

        for (auto i : allFibres.at(last).fibres) {

            if (allFibres.at(i.first).visited != true) {
                std::vector<Coord> newpath(route);
                newpath.push_back(i.first);
                queue.push_back(newpath);
                allFibres.at(i.first).visited = true;
            }
        }
    }
    return route;
}
// Palauttaa jonkin (mielivaltaisen) reitin annettujen pisteiden välillä.
// Palautetussa vektorissa on ensimmäisenä alkupiste hinnalla 0, sitten kaikki reitin varrella olevat pisteet ja
// kustannus ko. pisteeseen saakka, viimeisenä loppupiste. Jos reittiä ei löydy, palautetaan tyhjä vektori.
std::vector<std::pair<Coord, Cost>> Datastructures::route_any(Coord fromxpoint, Coord toxpoint)
{
    return route_least_xpoints(fromxpoint, toxpoint);
}

// Palauttaa annettujen pisteiden välillä reitin, jossa on mahdollisimman vähän risteyksiä
// (ja siis myös mahdollisimman vähän kuituja). Palautetussa vektorissa on ensimmäisenä alkupiste hinnalla 0,
// sitten kaikki reitin varrella olevat pisteet ja kustannus ko. pisteeseen saakka, viimeisenä loppupiste. Jos reittiä ei löydy, palautetaan tyhjä vektori
std::vector<std::pair<Coord, Cost>> Datastructures::route_least_xpoints(Coord fromxpoint, Coord toxpoint)
{
    if (allFibres.find(fromxpoint) == allFibres.end() or
        allFibres.find(toxpoint) == allFibres.end() ) {
        return {};
    }

    std::vector<std::pair<Coord, Cost>> route = {};
    std::vector<Coord> path = bfsRoute(fromxpoint, toxpoint);

    if (path.size() == 1) {
        path.clear();
    }

    if (!path.empty()) {
        route.push_back(std::make_pair(path.front(), 0));
        Coord prev = path.front();
        path.erase(path.begin());

        Cost total = 0;

        for (auto a : path) {
            Cost cost = allFibres.at(prev).fibres.at(a);
            total += cost;
            route.push_back(std::make_pair(a, total));
            prev = a;
        }
    }
    return route;
}

std::vector<std::pair<Coord, Cost>> Datastructures::route_fastest(Coord /*fromxpoint*/, Coord /*toxpoint*/)
{
    // Replace this with your implementation
    return {};
}

std::vector<Coord> Datastructures::route_fibre_cycle(Coord /*startxpoint*/)
{
    // Replace this with your implementation
    return {};
}

Cost Datastructures::trim_fibre_network()
{
    // Replace this with your implementation
    return NO_COST;
}
