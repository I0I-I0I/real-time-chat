import cls from "@/utils/cls"

import { Message } from "@/components/UI"
import { MessageType } from "@/types"

import styles from "./Chat.module.css"

interface ChatProps {
    className?: string
    data: MessageType[]
}

export const Chat = ({
    className = "",
    data
}: ChatProps) => {
    return (
        <ul className={cls(styles.messages, className)}>
            { data.map((item: MessageType, index: number): JSX.Element => (
                <Message key={index} variant={item.author === "me" ? "right" : "left"}>{item.text}</Message>
            )) }
        </ul>
    )
}
