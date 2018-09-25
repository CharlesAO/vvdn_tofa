///
/// @file
/// @copyright All code copyright Movidius Ltd 2018, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     DataQueue implementation
///

#ifndef DATA_QUEUE_H
#define DATA_QUEUE_H
#include <tuple>

template <class T> class DataQueue{
public:

    DataQueue(){
        consumerTargetFlag = 0;
        lastIdReturned = 0;
        lastIdConsumed = 0;
        keepData = false;
        q.clear();
    }

    void setKeep(bool keep){
        keepData = keep;
    }
    void registerConsumer(int consId){
        consumerTargetFlag |= (1 << consId);
    }
    void push(T elem, int id){

        q.push_back(std::make_tuple(elem,id,0));
    }
    T get(char consumer){
        int consumerFlag = 1 << consumer;
        //find first element which is not consumed, yet.
        auto it = std::find_if( std::begin( q ),
                                std::end( q ),
                                [&]( std::tuple<T,int,int> v ){return !(consumerFlag & std::get<2>(v)); } );
        if(it == std::end( q )){
            return NULL;
        }else{
            return std::get<0>(*it);
        }
    }
    T get(int id, char consumer){
        int consumerFlag = 1 << consumer;
        auto it = std::find_if( std::begin( q ),
                                std::end( q ),
                                [&]( std::tuple<T,int,int> v ){ return id == std::get<1>(v); } );
        if(it == std::end( q )){
            return NULL;
        }
        if((std::get<2>(*it) & consumerFlag) ){
            return NULL;
        }else{
            return std::get<0>(*it);
        }
    }
    void consume(int id, char consumer){
        int consumerFlag = 1 << consumer;
        auto it = std::find_if( std::begin( q ),
                                       std::end( q ),
                                       [&]( std::tuple<T,int,int> v ){ return id == std::get<1>(v); } );
        if(it == std::end( q )){
            //nothing to consume.
            return;
        }
        std::tuple<T,int,int>& elem = *it;
        std::get<2>(elem) |= consumerFlag;
        consumerFlag = std::get<2>(elem);
        // if an entry with a specific id was consumed by all consumers, remove all entries from the list with smaller ids
        if(consumerFlag == consumerTargetFlag ){
            std::tuple<T,int,int>& fr = q.front();

            while (std::get<1>(fr) < std::get<1>(elem) ){
                q.pop_front();
                freeElem(std::get<0>(fr));
                fr = q.front();
            }
            if(std::get<1>(fr) == std::get<1>(elem)){//consume last element
                q.pop_front();
                freeElem(std::get<0>(fr));
            }
        }
    }

    void clearQueue() {
        if(q.empty())
            return;
        std::tuple<T,int,int>& ba = q.back();
        /*
        q.clear();
        */
            std::tuple<T,int,int>& fr = q.front();

            while (std::get<1>(fr) < std::get<1>(ba) ){
                q.pop_front();
                freeElem(std::get<0>(fr));
                fr = q.front();
            }
            if(std::get<1>(fr) == std::get<1>(ba)){//consume last element
                q.pop_front();
                freeElem(std::get<0>(fr));
            }
    }
    bool empty(){
        return q.empty();
    }
    ~DataQueue(){

    }
private:
    std::tuple<T,int,int>& get(int id){
        auto it = std::find_if( std::begin( q ),
                                std::end( q ),
                                [&]( std::tuple<T,int,int> v ){ return id == std::get<1>(v); } );
        std::tuple<T,int,int>& ref = notFound ;
        if(it != std::end( q )){
            ref = *it;
        }
        return ref;

    }
    void freeElem(T elem){
        if (!keepData)
            delete elem;
    }
    std::tuple<T,int,int> notFound;

    std::list<std::tuple<T,int,int>> q;
    int consumerTargetFlag;
    int lastIdReturned;
    int lastIdConsumed;
    bool keepData;
};

#endif
