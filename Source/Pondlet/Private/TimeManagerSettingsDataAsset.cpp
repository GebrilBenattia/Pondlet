// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeManagerSettingsDataAsset.h"

//void UTimeManagerSettingsDataAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
//{
//    Super::PostEditChangeProperty(PropertyChangedEvent);
//
//    if (!bEnableSpecificTimeFormat)
//    {
//        bEnable24Format = false;
//        //bEnable12Format = false;
//        bEnableMinutes = false;
//        bEnableSeconds = false;
//        return;
//    }
//
//    if (!bEnable24Format/* && !bEnable12Format*/)
//    {
//        bEnableMinutes = false;
//        bEnableSeconds = false;
//        return;
//    }
//
//    if (!bEnableMinutes)
//    {
//        bEnableSeconds = false;
//    }
//
//    if (PropertyChangedEvent.Property)
//    {
//        const FName PropertyName = PropertyChangedEvent.Property->GetFName();
//
//        //if (PropertyName == GET_MEMBER_NAME_CHECKED(UTimeManagerSettingsDataAsset, bEnable24Format))
//        //{
//        //    if (bEnable24Format)
//        //    {
//        //        bEnable12Format = false; // Disable the other format
//        //    }
//        //}
//        //else if (PropertyName == GET_MEMBER_NAME_CHECKED(UTimeManagerSettingsDataAsset, bEnable12Format))
//        //{
//        //    if (bEnable12Format)
//        //    {
//        //        bEnable24Format = false; // Disable the other format
//        //    }
//        //}
//    }
//}
