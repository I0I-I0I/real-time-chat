import React, { useState } from "react"

type IUseInput = [
    {
        value: string
        onChange: (element: React.ChangeEvent<HTMLInputElement>) => void
    },
    setValue: (value: string) => void
]

function useInput(defaulValue: string): IUseInput {
    const [value, setValue] = useState(defaulValue)

    const onChange = (element: React.ChangeEvent<HTMLInputElement>) => {
        setValue(element.target.value)
    }

    return [
        {
            value,
            onChange,
        },
        setValue,
    ]
}

export default useInput
