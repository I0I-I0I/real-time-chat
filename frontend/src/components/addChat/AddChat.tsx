import cls from "@/utils/cls"
import styles from "./AddChat.module.css"
import { DropdownData, SearchWithDropdown } from "@/components/UI"
import { forwardRef, useEffect, useState } from "react"
import { IUser } from "@/types"
import SearchService from "@/api/SearchService"

export interface createNewChatData {
    id: number
    name: string
    login?: string
}

interface AddChatProps {
    className?: string
    createNewChat: (data: createNewChatData) => void
}

export const AddChat = forwardRef<HTMLDivElement, AddChatProps>(({
    className = "",
    createNewChat
}, ref) => {
    const [search, setSearch] = useState("")
    const [friends, setFriends] = useState<DropdownData[]>([])

    const onClickDropdownItem = (data: DropdownData) => {
        if (data.login == null) return
        createNewChat(data)
    }

    const getUsers = async (login: string) => {
        const data = await SearchService.search<IUser>("login", "users", login)
        if (data == null) {
            setFriends([])
            return
        }
        const dropdownData: DropdownData[] = data.map((user) => ({
            id: user.id,
            name: user.username,
            login: user.login
        }))
        setFriends(dropdownData)
    }

    useEffect(() => {
        if (search === "") {
            setFriends([])
            return
        }
        getUsers(search)
    }, [search])

    return (
        <div className={cls(styles.add_chat, className)} ref={ref}>
            <SearchWithDropdown setPrompt={setSearch} data={friends} onClickDropdownItem={onClickDropdownItem} />
        </div>
    )
})
